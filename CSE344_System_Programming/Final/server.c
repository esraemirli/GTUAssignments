#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include "graph.c"

#define MAX 3500
#define SA struct sockaddr

void freeGarbages();
void timeStamp();
void make_daemon();
int findSlot(int *, int, int);

struct Cache
{
    int *path;
    int src;
    int dest;
    int pathSize;
};

struct threadParam
{
    int id;
    int *connection;
    struct Cache *cache;
    int *cindex;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _read_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int *globalConnection = NULL;
int *globalCindex = NULL;
struct Cache *globalCache = NULL;
int runningThreadNumber;
int minThread, maxThread;
int cacheSize = 100;
int pid;
int PORT;
time_t clk;
static FILE *outfp;
pthread_t *pthreads;
struct Graph *graph;
int threadNumber = 0;
int ctrlc = 0;

void intHandler()
{
    timeStamp();
    fprintf(outfp, "Termination signal received, waiting for ongoing threads to complete.\n");
    ctrlc = 1;
    pthread_cond_broadcast(&cond);
    int loopv;

    for (loopv = 0; loopv < minThread; loopv++)
    {
        pthread_join(pthreads[loopv], NULL);
    }

    if (runningThreadNumber == 0)
    {
        timeStamp();
        fprintf(outfp, "All threads have terminated, server shutting down.\n");

        fclose(outfp);
        freeGarbages();
        kill(pid, SIGTERM);
    }
}

void *func(void *paramsockfd)
{
    struct threadParam thParam = *((struct threadParam *)paramsockfd);

    int sockfd;
    int localThreadNum;
    char buff[MAX];
    int localIndex;

    pthread_mutex_lock(&mutex);
    localThreadNum = ++threadNumber;
    pthread_mutex_unlock(&mutex);

    while (ctrlc == 0)
    {
        pthread_mutex_lock(&_read_mutex);
        localIndex = findSlot(thParam.connection, maxThread * 3, 1);

        if (localIndex == -1)
        {

            timeStamp();
            fprintf(outfp, "Thread #%d: waiting for connection\n", localThreadNum);
            pthread_cond_wait(&cond, &_read_mutex);
            localIndex = findSlot(thParam.connection, maxThread * 3, 1);

            sockfd = *(thParam.connection + localIndex);
            if (localIndex != -1)
                *(thParam.connection + localIndex) = -1;
        }
        else
        {
            sockfd = *(thParam.connection + localIndex);
            *(thParam.connection + localIndex) = -1;
        }
        pthread_mutex_unlock(&_read_mutex);

        if (ctrlc == 0 && localIndex != -1)
        {
            pthread_mutex_lock(&mutex);
            runningThreadNumber++;
            pthread_mutex_unlock(&mutex);

            double persent = ((double)runningThreadNumber / (double)minThread) * 100;
            timeStamp();
            fprintf(outfp, "A connection has been delegated to thread id #%d, system load %.1f%%\n", localThreadNum, persent);

            bzero(buff, MAX);

            read(sockfd, buff, sizeof(buff));
            int i1, i2;
            sscanf(buff, "%d %d ", &i1, &i2);

            int i;

            int inchache = 0, sendClient = -1;
            timeStamp();
            fprintf(outfp, "Thread #%d: searching database for a path from node %d to node %d\n", localThreadNum, i1, i2);

            pthread_mutex_lock(&mutex);
            for (i = 0; i < *thParam.cindex; i++)
            {
                if (thParam.cache[i].dest == i2 && thParam.cache[i].src == i1)
                {
                    inchache = 1;
                    sendClient = i;
                    if (thParam.cache[i].pathSize == 0 || thParam.cache[i].path == NULL)
                    {
                        timeStamp();
                        fprintf(outfp, "Thread #%d: no path in database, calculating %d->%d\n", localThreadNum, i1, i2);
                    }
                    else
                    {
                        int j;
                        timeStamp();
                        fprintf(outfp, "Thread #%d: path found in database: ", localThreadNum);
                        for (j = 0; j < thParam.cache[i].pathSize; j++)
                        {
                            fprintf(outfp, "%d", thParam.cache[i].path[j]);
                            if (j < thParam.cache[i].pathSize - 1)
                                fprintf(outfp, "->");
                        }
                        fprintf(outfp, "\n");
                    }
                }
            }
            pthread_mutex_unlock(&mutex);

            //if not exists in cache
            if (inchache == 0)
            {
                // search in graph
                struct Result result = findPath(graph, i1, i2);
                // write to cache
                pthread_mutex_lock(&mutex);
                if (cacheSize <= *thParam.cindex)
                {
                    thParam.cache = realloc(thParam.cache, (cacheSize * 2) * sizeof(int));
                    cacheSize *= 2;
                }

                thParam.cache[*thParam.cindex].src = i1;
                thParam.cache[*thParam.cindex].dest = i2;
                thParam.cache[*thParam.cindex].pathSize = result.size;
                thParam.cache[*thParam.cindex].path = NULL;
                if (result.size != 0 && result.arr[0] != -1)
                {
                    thParam.cache[*thParam.cindex].path = (int *)malloc(sizeof(int) * result.size);

                    timeStamp();
                    fprintf(outfp, "Thread #%d: path calculated: ", localThreadNum);
                    for (i = 0; i < result.size; i++)
                    {
                        thParam.cache[*thParam.cindex].path[i] = result.arr[i];
                        fprintf(outfp, "%d", thParam.cache[*thParam.cindex].path[i]);
                        if (i < result.size - 1)
                            fprintf(outfp, "->");
                    }
                    fprintf(outfp, "\n");
                }
                else
                {
                    timeStamp();
                    fprintf(outfp, "Thread #%d: path not possible from node %d to %d\n", localThreadNum, i1, i2);
                }
                sendClient = *thParam.cindex;
                (*thParam.cindex)++;

                pthread_mutex_unlock(&mutex);
            }

            timeStamp();
            fprintf(outfp, "Thread #%d: responding to client and adding path to database\n", localThreadNum);
            write(sockfd, &thParam.cache[sendClient].pathSize, sizeof(thParam.cache[sendClient].pathSize));

            int fclient[thParam.cache[sendClient].pathSize];
            for (i = 0; i < thParam.cache[sendClient].pathSize; i++)
            {
                fclient[i] = thParam.cache[sendClient].path[i];
            }

            write(sockfd, fclient, sizeof(fclient));

            bzero(buff, MAX);

            pthread_mutex_lock(&mutex);
            runningThreadNumber--;
            localIndex = -1;
            close(sockfd);
            pthread_mutex_unlock(&mutex);
        }
    }

    globalConnection = thParam.connection;
    globalCindex = thParam.cindex;
    globalCache = thParam.cache;

    pthread_exit(NULL);
}
// Driver function
int memSize;
int main(int argc, char *argv[])
{
    char *pathToFile;
    char *pathToLogFile;
    int opt;

    while ((opt = getopt(argc, argv, ":i:p:o:s:x:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            pathToFile = optarg;
            break;
        case 'o':
            pathToLogFile = optarg;
            break;
        case 'p':
            PORT = atoi(optarg);
            break;
        case 's':
            minThread = atoi(optarg);
            break;
        case 'x':
            maxThread = atoi(optarg);
            break;
        case ':':
            fprintf(stderr, "Option needs a value %s\n", optarg);
            fprintf(stderr, "USAGE : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
            return -1;
        case '?':
            fprintf(stderr, "Unknown option: %c\n", optopt);
            fprintf(stderr, "USAGE : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
            return -1;
        }
    }
    if (optind < 10)
    {
        fprintf(stderr, "USAGE : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
        return -1;
    }
    for (; optind < argc; optind++)
    {
        fprintf(stderr, "Extra arguments: %s\n", argv[optind]);
        fprintf(stderr, "USAGE : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
        return -1;
    }
    if (minThread < 2)
    {
        fprintf(stderr, "s is the number of threads in the pool at startup (at least 2)\n");
        return -1;
    }
    if (maxThread < minThread)
    {
        fprintf(stderr, "x >= s\n");
        return -1;
    }

    // make deamon process
    make_daemon();

    pid = getpid();
    signal(SIGINT, intHandler);

    struct threadParam _threadParam;
    runningThreadNumber = 0;
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    //READ GRAPH TO DS
    FILE *fp;

    char *line = NULL;
    size_t flen = 0;
    ssize_t read;

    fp = fopen(pathToFile, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Not found %s.\n", pathToFile);
        return -1;
    }

    outfp = fopen(pathToLogFile, "w");
    if (outfp == NULL)
    {
        fprintf(stderr, "Not found %s.\n", pathToLogFile);
        return -1;
    }

    int size = 0;
    memSize = 10;

    timeStamp();
    fprintf(outfp, "Executing with parameters:\n");
    timeStamp();
    fprintf(outfp, "-i %s\n", pathToFile);
    timeStamp();
    fprintf(outfp, "-p %d\n", PORT);
    timeStamp();
    fprintf(outfp, "-o %s\n", pathToLogFile);
    timeStamp();
    fprintf(outfp, "-s %d\n", minThread);
    timeStamp();
    fprintf(outfp, "-x %d\n", maxThread);

    int *vertices = (int *)malloc(memSize * sizeof(int));

    timeStamp();
    fprintf(outfp, "Loading graph...\n");

    struct timeval stop, start;
    gettimeofday(&start, NULL);
    while ((read = getline(&line, &flen, fp)) != -1)
    {
        if (line[0] != '#')
        {

            int sN, dN;
            sscanf(line, "%d %d", &sN, &dN);
            int sNExist = 0, dNExist = 0;
            int i;
            for (i = 0; i < size; i++)
            {
                if (vertices[i] == sN)
                {
                    sNExist = 1;
                }
                if (vertices[i] == dN)
                {
                    dNExist = 1;
                }
            }
            if (size >= memSize)
            {
                vertices = realloc(vertices, (memSize * 2) * sizeof(int));
                memSize *= 2;
            }
            if (!sNExist)
                vertices[size++] = sN;
            if (!dNExist)
                vertices[size++] = dN;
        }
    }
    int verticesNum = size;
    free(vertices);
    vertices = NULL;
    fclose(fp);

    if (size == 0)
    {
        timeStamp();
        fprintf(outfp, "File is empty. \n");
        free(line);
        fclose(outfp);
        return -1;
    }
    //Fill graph
    int edges = 0;
    graph = createGraph(verticesNum);
    fp = fopen(pathToFile, "r");
    while ((read = getline(&line, &flen, fp)) != -1)
    {
        if (line[0] != '#')
        {
            edges++;
            int sN, dN;
            sscanf(line, "%d %d", &sN, &dN);
            addEdge(graph, sN, dN);
        }
    }
    fclose(fp);
    free(line);

    //time
    gettimeofday(&stop, NULL);
    double secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    timeStamp();
    fprintf(outfp, "Graph loaded in %f seconds with %d nodes and %d edges.\n", secs, verticesNum, edges);

    /*********************************************************/
    pthreads = (pthread_t *)malloc(sizeof(pthread_t) * maxThread);

    _threadParam.connection = (int *)malloc(sizeof(int) * maxThread * 3);
    _threadParam.cache = (struct Cache *)malloc(sizeof(int) * cacheSize);
    _threadParam.cindex = (int *)malloc(sizeof(int));
    *_threadParam.cindex = 0;

    int ii;
    for (ii = 0; ii < maxThread * 3; ii++)
    {
        *(_threadParam.connection + ii) = -1;
    }

    timeStamp();
    fprintf(outfp, "A pool of %d threads has been created.\n", minThread);
    for (ii = 0; ii < minThread; ii++)
    {
        _threadParam.id = ii;
        pthread_create(&pthreads[ii], NULL, func, &_threadParam);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        timeStamp();
        fprintf(outfp, "socket creation failed...\n");
        freeGarbages();
        fclose(outfp);
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        timeStamp();
        fprintf(outfp, "socket bind failed...\n");
        freeGarbages();
        fclose(outfp);
        exit(0);
    }

    if ((listen(sockfd, 3)) != 0)
    {
        timeStamp();
        fprintf(outfp, "Listen failed...\n");
        freeGarbages();
        fclose(outfp);
        exit(0);
    }
    len = sizeof(cli);

    while (1)
    {
        // wait for client
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (runningThreadNumber >= minThread * 0.75 && ctrlc != 1)
        {
            //create new thread..
            int border = minThread + minThread * 0.25;
            int ii;
            for (ii = minThread; ii < border && ii < maxThread; ii++)
            {
                pthread_create(&pthreads[ii], NULL, func, &_threadParam);
            }
            double percent = ((double)runningThreadNumber / (double)minThread) * 100;
            if (border >= maxThread)
            {
                minThread = maxThread;
            }
            else
            {
                minThread += minThread * 0.25;
            }
            timeStamp();
            fprintf(outfp, "System load %.1f%%, pool extended to %d threads\n", percent, minThread);
            if (runningThreadNumber >= maxThread)
            {
                timeStamp();
                fprintf(outfp, "No thread is available! Waiting for one.\n");
            }
        }

        if (connfd < 0)
        {
            timeStamp();
            fprintf(outfp, "server acccept failed...\n");
            freeGarbages();
            fclose(outfp);
            exit(0);
        }

        pthread_mutex_lock(&_read_mutex);
        int rnd = findSlot(_threadParam.connection, maxThread * 2, -1);
        *(_threadParam.connection + rnd) = connfd;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&_read_mutex);
    }
    int pjoin = 0;
    for (; pjoin < minThread; pjoin++)
    {
        pthread_join(pthreads[pjoin], NULL);
    }

    close(sockfd);
    fclose(outfp);
    freeGarbages();
    return 0;
}

int findSlot(int *arr, int size, int order)
{
    int i;
    if (order == -1)
    {
        int random = 0;

        while (1)
        {
            srand(time(0));
            random = rand() % ((maxThread * 2) - 1);
            if (*(arr + random) == -1)
                return random;
        }
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            if (*(arr + i) != -1)
            {
                return i;
            }
        }
    }
    return -1;
}

void freeGarbages()
{
    int i;
    free(pthreads);
    for (i = 0; i < graph->V; i++)
    {
        while (graph->array[i].head != NULL)
        {
            struct AdjListNode *temp = graph->array[i].head->next;
            free(graph->array[i].head);
            graph->array[i].head = temp;
        }
    }

    free(graph->array);

    free(graph);
    if (globalConnection != NULL)
        free(globalConnection);

    if (globalCindex != NULL)
        for (i = 0; i < *globalCindex; i++)
        {
            if (globalCache[i].path != NULL)
                free(globalCache[i].path);
        }

    if (globalCindex != NULL)
        free(globalCindex);

    if (globalCache != NULL)
        free(globalCache);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&_read_mutex);
    pthread_cond_destroy(&cond);
}

void timeStamp()
{
    clk = time(NULL);
    char *time = ctime(&clk);
    int i = 0;
    while (time[i] != '\0')
    {
        if (time[i] == '\n')
        {
            time[i] = '\0';
        }
        i++;
    }
    fprintf(outfp, "%s : ", time);
}

void make_daemon()
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    int fds;
    for (fds = sysconf(_SC_OPEN_MAX); fds >= 0; fds--)
    {
        close(fds);
    }

    openlog("MorningStar", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "pid : %d", getpid());
}