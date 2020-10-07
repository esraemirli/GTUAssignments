#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "graph.c"
#include <arpa/inet.h>

#define MAX 3500

#define SA struct sockaddr
int i1, i2;
int PORT;
time_t clk;
struct tm *stm;

void timeStamp();

void func(int sockfd)
{
    char buff[MAX];

    bzero(buff, sizeof(buff));

    char src[MAX], dest[MAX];
    sprintf(src, "%d", i1);
    sprintf(dest, "%d", i2);
    timeStamp();
    fprintf(stderr, "Client (%d) connected and requesting a path from node %d to %d\n", getpid(), i1, i2);

    strcat(src, " ");
    strcat(buff, src);
    strcat(buff, dest);
    //time
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    //send i1 and i2
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));

    //take path info
    int size;
    read(sockfd, &size, sizeof(int));
    int path[size];

    bzero(path, size);
    read(sockfd, path, sizeof(path));

    //time
    gettimeofday(&stop, NULL);
    double secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    if (size == 0)
    {
        timeStamp();
        fprintf(stderr, "Server’s response (%d): NO PATH, arrived in %lf seconds, shutting down\n", getpid(), secs);
    }
    else
    {
        timeStamp();
        fprintf(stderr, "Server’s response to (%d): ", getpid());
        int i;
        for (i = 0; i < size; i++)
        {
            fprintf(stderr, "%d", path[i]);
            if (i < size - 1)
                fprintf(stderr, "->");
        }
        printf(", arrived in %lf seconds.\n", secs);
    }
}

int main(int argc, char *argv[])
{
    //./client -a 127.0.0.1 -p PORT -s 768 -d 979
    char *IP;
    int opt;

    while ((opt = getopt(argc, argv, ":a:p:s:d:")) != -1)
    {
        switch (opt)
        {
        case 'a':
            IP = optarg;
            break;
        case 'p':
            PORT = atoi(optarg);
            break;
        case 's':
            i1 = atoi(optarg);
            break;
        case 'd':
            i2 = atoi(optarg);
            break;
        case ':':

            fprintf(stderr, "Option needs a value %s\n", optarg);
            fprintf(stderr, "USAGE : ./client -a IPADDRESS -p PORT -s SOURCE_NODE_NUMBER -d DESTINATION_NODE_NUMBER\n");
            return -1;
        case '?':
            fprintf(stderr, "Unknown option: %c\n", optopt);
            fprintf(stderr, "USAGE : ./client -a IPADDRESS -p PORT -s SOURCE_NODE_NUMBER -d DESTINATION_NODE_NUMBER\n");
            return -1;
        }
    }
    if (optind < 8)
    {
        fprintf(stderr, "USAGE : ./client -a IPADDRESS -p PORT -s SOURCE_NODE_NUMBER -d DESTINATION_NODE_NUMBER\n");
        return -1;
    }
    for (; optind < argc; optind++)
    {
        fprintf(stderr, "Extra arguments: %s\n", argv[optind]);
        fprintf(stderr, "USAGE : ./client -a IPADDRESS -p PORT -s SOURCE_NODE_NUMBER -d DESTINATION_NODE_NUMBER\n");
        return -1;
    }

    if (i1 < 0 || i2 < 0)
    {
        fprintf(stderr, "SOURCE_NODE_NUMBER or DESTINATION_NODE_NUMBER can not be negative numbers\n");
        return -1;
    }

    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        timeStamp();
        fprintf(stderr, "socket creation failed...\n");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        timeStamp();
        fprintf(stderr, "Client (%d) could not connect to %s:%d\n", getpid(), IP, PORT);
        exit(0);
    }
    else
    {
        timeStamp();
        fprintf(stderr, "Client (%d) connecting to %s:%d\n", getpid(), IP, PORT);
    }
    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
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
    fprintf(stderr, "%s : ", time);
}