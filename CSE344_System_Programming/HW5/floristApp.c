#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <signal.h>

#define SIZE 255
struct Florist
{
    int threadId;
    char name[SIZE];
    double x, y;
    double click;
    char flowers[SIZE][SIZE]; //names
    int typeNumber;           //kaç çeşit çiçek

    char **queue;      //müşteriler
    char **delivery;      //sipariş edilen çiçek
    double *distance;     //her müşterinin mesafesi
    int *clientNumber; //kaç müşteri (queue size)

    int limitFull;
};

struct Statistic
{
    char name[SIZE];
    int sales;
    int totalTime;
};


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond4thread = PTHREAD_COND_INITIALIZER;
int MEMSIZE = 1000;

int cv = 1; //client biterse 0 olacak
int numOfThread = 0;
int threadCount = 0;

void *threadFun(void* );
int exitProgram(struct Florist[], int, pthread_t[] );


int ctrlc = 0;
void intHandler() {
    ctrlc = 1;
    fprintf(stdout,"CTRL-C signal was catched, the program is shuts down gracefully.Wait a little.\n");
}

int main(int argc, char *argv[])
{
    char* filePath;
    int opt;

    while ((opt = getopt(argc, argv, ":i:")) != -1)
    {
      switch (opt)
      {
      case 'i':
         filePath = optarg;
         break;
      case ':':
         fprintf(stderr,"Option needs a value %s\n", optarg);
         fprintf(stderr,"USAGE : ./program -i filePath\n");
         return -1;
      case '?':
         fprintf(stderr,"Unknown option: %c\n", optopt);
         fprintf(stderr,"USAGE : ./program -i filePath\n");
         return -1;
      }
    }
    if(optind < 3) {
        fprintf(stderr,"USAGE : ./program -i filePath\n");
        return -1;
    }
    for (; optind < argc; optind++)
    {
        fprintf(stderr,"Extra arguments: %s\n", argv[optind]);
        fprintf(stderr,"USAGE : ./program -i filePath\n");
        return -1;
    }

    FILE *fp = fopen(filePath, "r");
    if (!fp)
    {
        fprintf(stderr, "Error opening file %s\n",filePath);
        return EXIT_FAILURE;
    }
    fprintf(stdout,"Florist application initializing from file: %s\n",filePath);
    signal(SIGINT, intHandler);
    
    size_t NUMTHREADS = 50;
    struct Florist florists[NUMTHREADS];
    char floristName[SIZE];
    double floristX = 0.0;
    double floristY = 0.0;
    double floristClick = 0.0;
    char floristFlowers[SIZE][SIZE];

    pthread_t threads[NUMTHREADS];
    int emptyLine = 0;
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int first = 0;
    int noFlorist = 0;
    while ( (getline(&line_buf, &line_buf_size, fp) >= 0 ) && ctrlc != 1 )
    {   
        if(line_buf[0] == '\n' && noFlorist == 0) {
            fprintf(stderr,"The file is not suitable for format. The program is terminating.\n");
            exitProgram(florists,numOfThread,threads);
        }
        noFlorist = 1;
        if (line_buf[0] == '\n')
        {
            emptyLine++;
           
            pthread_mutex_lock(&mutex);
            if (emptyLine == 1)
            {
                pthread_cond_wait(&cond4thread, &mutex);
            }
            pthread_mutex_unlock(&mutex);
            continue;
        }

        if (emptyLine == 0)
        {
            sscanf(line_buf, "%s "
                             "(%lf "
                             ",%lf "
                             "; %lf) ",
                   floristName, &floristX, &floristY, &floristClick);
            if(floristClick == 0) {
                fprintf(stderr,"The file is not suitable for format. The program is terminating.\n");
                exitProgram(florists,numOfThread,threads);
            }
            int i = 0;
            int j = 0;
            int ikiNokta = 0;
            int cicekSira = 0;
            int cicekAd = 0;
            while (line_buf[i] != '\0')
            {
                if (line_buf[i] == ':')
                {
                    ikiNokta = 1;
                }
                if (ikiNokta == 0)
                {
                    i++;
                    continue;
                }
                if ( (line_buf[i] >= 'a' && line_buf[i] <= 'z') || (line_buf[i] >= 'A' && line_buf[i] <= 'Z') )
                {
                    floristFlowers[cicekSira][cicekAd++] = line_buf[i];
                }
                if (line_buf[i] == ',')
                {
                    floristFlowers[cicekSira][cicekAd] = '\0';
                    cicekSira++;
                    cicekAd = 0;
                }
                i++;
            }

            cicekSira++;
            if ( ! ((floristName[0] >= 'a' && floristName[0] <= 'z') || (floristName[0] >= 'A' && floristName[0] <= 'Z')) ) {
                fprintf(stderr,"Florist name(%s) is not valid. The program is terminating.\n",floristName);
                exitProgram(florists,numOfThread,threads);
            }
            //florists[numOfThread].threadId = numOfThread;
            strcpy(florists[numOfThread].name, floristName);
            florists[numOfThread].x = floristX;
            florists[numOfThread].y = floristY;
            florists[numOfThread].click = floristClick;
            florists[numOfThread].typeNumber = cicekSira;
            floristClick = 0.0;
            for (j = 0; j < cicekSira; j++)
            {
                strcpy(florists[numOfThread].flowers[j], floristFlowers[j]);
                int l;
                for (l = 0; l < SIZE; l++)
                {
                    floristFlowers[j][l] = '\0';
                } 
            }
           
            //müşteri adları için yer al..
            int n;
            florists[numOfThread].queue = (char**) malloc(MEMSIZE * sizeof(char *));
            for (n = 0; n < MEMSIZE; n++)
                florists[numOfThread].queue[n] = malloc(50 * sizeof(char));

            //müşterilerin siparişleri için yer al.
            florists[numOfThread].delivery = (char**) malloc(MEMSIZE * sizeof(char *));
            for (n = 0; n < MEMSIZE; n++)
                florists[numOfThread].delivery[n] = malloc(50 * sizeof(char));

            //müşterinin mesafeleri için yer al..
            florists[numOfThread].distance = (double*) malloc(MEMSIZE * sizeof(double));

            //kaç müşteri sırada onun için yer al..
            florists[numOfThread].clientNumber = (int*) malloc(MEMSIZE * sizeof(int));
            *(florists[numOfThread].clientNumber) = 0;
        
            //ilk olarak 0, 1 olunca kaydetmez..
            florists[numOfThread].limitFull = 0;
            pthread_create(&threads[numOfThread], NULL, threadFun, &florists[numOfThread]);

            florists[numOfThread].threadId = threads[numOfThread];
            numOfThread++;
        }
        if (emptyLine == 1)
        {
            if(first == 0) {
                fprintf(stdout,"%d florists have been created\n",numOfThread);
                fprintf(stdout,"Processing requests\n");
                first = 1;
            }
            double clientX, clientY;
            char clientName[255] = "\0";
            char clientFlower[255] = "\0";

            sscanf(line_buf, "%s "
                             "(%lf "
                             ",%lf): "
                             "%s",
                   clientName, &clientX, &clientY, clientFlower);
            pthread_mutex_lock(&mutex);
        
            double tempCb = INT_MAX;
            int k;
            double cbX, cbY;
            int floristIndex = -1;
            int hasFlower;
            int m;
            for (k = 0; k < numOfThread; k++)
            {
                hasFlower = 0;
                for (m = 0; m < florists[k].typeNumber; m++)
                {
                    if (strcmp(florists[k].flowers[m], clientFlower) == 0)
                        hasFlower = 1;
                }

                if (hasFlower == 1)
                {
                    cbX = clientX - florists[k].x;
                    cbY = clientY - florists[k].y;

                    if (cbY < 0)
                        cbY *= -1;

                    if (cbX < 0)
                        cbX *= -1;

                    if (cbX >= cbY && cbX < tempCb)
                    {
                        tempCb = cbX;
                        floristIndex = k;
                    }

                    if (cbY >= cbX && cbY < tempCb)
                    {
                        tempCb = cbY;
                        floristIndex = k;
                    }
                }
            }
            if( !( (florists[floristIndex].name[0] >= 'a' && florists[floristIndex].name[0] <= 'z') || (florists[floristIndex].name[0] >= 'A' && florists[floristIndex].name[0] <= 'Z') ) ) {
                fprintf(stderr,"There are no florists selling '%s' flower.The program is terminating.\n",clientFlower);
                exitProgram(florists,numOfThread,threads);
            }
           
            if(*florists[floristIndex].clientNumber >= MEMSIZE -1) { //girmesin diye..
                if(florists[floristIndex].limitFull == 0) {
                    fprintf(stderr,"The limit of 1000 clients has been reached for florist %s. After %s, no service can be provided.\n",florists[floristIndex].name,clientName);
                    florists[floristIndex].limitFull = 1;
                }
            } else {
                strcpy(florists[floristIndex].queue[*(florists[floristIndex].clientNumber)], clientName);
                strcpy(florists[floristIndex].delivery[*(florists[floristIndex].clientNumber)], clientFlower); // ~~
                florists[floristIndex].distance[*(florists[floristIndex].clientNumber)] = tempCb;
                (*florists[floristIndex].clientNumber)++;
            }
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&mutex);
        }
    }
    cv = 0;
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    /* Free the allocated line buffer */
    fclose(fp);
    filePath = NULL;
    fp = NULL;
    free(line_buf);
    line_buf = NULL;
   
    /*******************************/
    struct Statistic result[numOfThread];
    void* thread_result;
    int i,q = 0;
    for (q = 0; q < numOfThread; q++) {
        pthread_join(threads[q], &thread_result);
        result[q] = *((struct Statistic *)thread_result);
        pthread_cancel(threads[q]);
    }
    fprintf(stdout,"All requests processed.\n");
    for (i = 0; i < numOfThread; i++)
    {
        fprintf(stdout,"%s closing shop.\n",result[i].name);
    }

    fprintf(stdout,"Sale statistics for today: \n");
    fprintf(stdout,"--------------------------------------\n");
    fprintf(stdout,"Florist\t # of sales\tTotal time\n");
    fprintf(stdout,"--------------------------------------\n");
    for (i = 0; i < numOfThread; i++)
    {
        fprintf(stdout,"%s\t %d\t\t%dms\n",result[i].name, result[i].sales, result[i].totalTime);
    }
    fprintf(stdout,"--------------------------------------\n");
    
    /* Close the file now that we are done with it */
    free(thread_result);

    int j;
    for (i = 0; i < numOfThread; i++)
    {
        for(j = 0; j < MEMSIZE; j++) {
            free(florists[i].queue[j]);
            free(florists[i].delivery[j]);
        }
        free(florists[i].queue);
        free(florists[i].delivery);
        free(florists[i].clientNumber);
        free(florists[i].distance);
    }
     
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond4thread);

    return EXIT_SUCCESS;
}

void *threadFun(void *param)
{                
    srand(time(NULL));
    int totalTime = 0;
    struct Florist florist = *((struct Florist *)param);
    int suAnkiClient = 1;
    threadCount++;
    while ( (cv == 1 || strcmp(florist.queue[suAnkiClient-1],"\0") != 0) && ctrlc != 1)
    {
        pthread_mutex_lock(&mutex);
        
        if (threadCount == numOfThread)
            pthread_cond_signal(&cond4thread);
    
        if (strcmp(florist.queue[suAnkiClient-1],"\0") == 0 && cv == 1)
        {    
            pthread_cond_wait(&cond, &mutex);
        }
        
        int preparationTime = (rand() % 250) + 1;
        double deliveryTime = florist.click * florist.distance[suAnkiClient-1];
        int sleepTime = preparationTime+deliveryTime;

        if(! florist.queue[suAnkiClient-1][0] == '\0') {

            fprintf(stdout,"Florist %s has delivered a %s to %s in %d\n",florist.name,florist.delivery[suAnkiClient-1],florist.queue[suAnkiClient-1],sleepTime);
            usleep(sleepTime);
            suAnkiClient++;

            totalTime += preparationTime + deliveryTime;
        }
        pthread_mutex_unlock(&mutex);
    }

    // burda da clientNumber ı kac kisiye hizmet ettiye ata
    struct Statistic *statistic = malloc(sizeof(struct Statistic));
    strcpy(statistic->name,florist.name);
    statistic->sales = *florist.clientNumber;
    statistic->totalTime = totalTime;

    pthread_exit(statistic);
}

int exitProgram(struct Florist florists[], int numOfThread, pthread_t threads[]){
    int i,j;
    for (i = 0; i < numOfThread; i++)
    {
        for(j = 0; j < MEMSIZE; j++) {
            free(florists[i].queue[j]);
            free(florists[i].delivery[j]);
        }
        free(florists[i].queue);
        free(florists[i].delivery);
        free(florists[i].clientNumber);
        free(florists[i].distance);
    }
    for ( i = 0; i < numOfThread; i++)
    {
        pthread_cancel(threads[i]);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond4thread);

    exit(EXIT_SUCCESS);
}