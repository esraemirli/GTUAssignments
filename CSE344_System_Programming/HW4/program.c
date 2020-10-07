#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/file.h>

typedef struct ingredients
{
    int f, w, s, m, i;
    int *ingp;
} y1;

sem_t mutex;
sem_t mainmutex;
int x, y;
//int _f, _w, _s, _m;
int lcv = 1;

void *funcTH(void *arg);

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
    
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    pthread_t tid5;
    pthread_t tid6;

    //file'da hata var mı, line 10'dan az mı önce kontrol et..
    int fdRead;
    fdRead = open(filePath, O_RDONLY);
    if (fdRead == -1)
    {
        perror("Inputpath Error: ");
        return -1;
    }

    unsigned char buf[3]; //w m \n
    int line = 0;
    char c1, c2, c3;
    ssize_t bytes_read = read(fdRead, buf, sizeof(buf));
    int newLine = 0;
    while (bytes_read == 2 || bytes_read == 3)
    {
        c1 = buf[0];
        c2 = buf[1];
        c3 = buf[2];

        if (c1 == c2)
        {
            if (c1 != '\n')
            {
                fprintf(stderr, "%d. line is not in the expected format !\n", line + 1);
                return -1;
            }
            if(c1 == '\n') {
                newLine = 1;
                break;
            }
        }
        else if ((c1 != 'M' && c1 != 'F' && c1 != 'S' && c1 != 'W') || (c2 != 'M' && c2 != 'F' && c2 != 'S' && c2 != 'W'))
        {
            fprintf(stderr, "%d. line is not in the expected format !\n", line + 1);
            return -1;
        }
        if (c3 != '\n')
        {
            fprintf(stderr, "%d. line is not in the expected format !\n", line + 1);
            return -1;
        }
        if (c3 == '\n' && c2 != '\n')
        {
            line++;
        }
        bytes_read = read(fdRead, buf, sizeof(buf));
    }
    if(newLine == 1) {
        fprintf(stderr, "%d. line is not in the expected format !\n", line + 1);
        return -1;
    }
    if (bytes_read == 1 && buf[0] != '\n' && line >= 10)
    {
        fprintf(stderr, "%d. line is not in the expected format !\n", line + 1);
        return -1;
    }
    if (line < 10)
    {
        fprintf(stderr, "\nLine cannot be less than 10 !");
        return -1;
    }
    close(fdRead);
    /*************************************************/
    sem_init(&mutex, 0, 1);
    sem_init(&mainmutex, 0, 0);
    int *ing = (int *)malloc(4 * sizeof(int));
    y1 str1;
    str1.i = 1;
    str1.f = 1;
    str1.w = 1;
    str1.s = 0;
    str1.m = 0;
    str1.ingp = ing;
    pthread_create(&tid1, NULL, funcTH, &str1);
    y1 str2;
    str2.i = 2;
    str2.f = 1;
    str2.w = 0;
    str2.s = 1;
    str2.m = 0;
    str2.ingp = ing;
    pthread_create(&tid2, NULL, funcTH, &str2);
    y1 str3;
    str3.i = 3;
    str3.f = 1;
    str3.w = 0;
    str3.s = 0;
    str3.m = 1;
    str3.ingp = ing;
    pthread_create(&tid3, NULL, funcTH, &str3);
    y1 str4;
    str4.i = 4;
    str4.f = 0;
    str4.w = 1;
    str4.s = 1;
    str4.m = 0;
    str4.ingp = ing;
    pthread_create(&tid4, NULL, funcTH, &str4);
    y1 str5;
    str5.i = 5;
    str5.f = 0;
    str5.w = 1;
    str5.s = 0;
    str5.m = 1;
    str5.ingp = ing;
    pthread_create(&tid5, NULL, funcTH, &str5);
    y1 str6;
    str6.i = 6;
    str6.f = 0;
    str6.w = 0;
    str6.s = 1;
    str6.m = 1;
    str6.ingp = ing;
    pthread_create(&tid6, NULL, funcTH, &str6);
    /******************* OKU *******************************/
    fdRead = open(filePath, O_RDONLY);
    bytes_read = read(fdRead, buf, sizeof(buf));
    if (fdRead == -1)
    {
        perror("Inputpath Error: ");
        return -1;
    }
    while (bytes_read == 2 || bytes_read == 3)
    {
        c1 = buf[0];
        c2 = buf[1];
        c3 = buf[2];
        if (c1 == c2)
        {
            break;
        }

        //printf("%c%c%c", c1, c2, c3);
        if (c1 == 'S')
            ing[2] = 1;
        else if (c1 == 'W')
            ing[1] = 1;
        else if (c1 == 'F')
            ing[0] = 1;
        else if (c1 == 'M')
            ing[3] = 1;

        if (c2 == 'S')
            ing[2] = 1;
        else if (c2 == 'W')
            ing[1] = 1;
        else if (c2 == 'F')
            ing[0] = 1;
        else if (c2 == 'M')
            ing[3] = 1;
       // fprintf(stderr, " w = %d s:%d f:%d m:%d\n", ing[1], ing[2], ing[0], ing[3]);

        sem_wait(&mainmutex);   
        bytes_read = read(fdRead, buf, sizeof(buf));
    }
    close(fdRead);

    lcv = 0;

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    pthread_join(tid6, NULL);

    sem_destroy(&mutex);
    sem_destroy(&mainmutex);
    free(ing);

    return 0;
}


void *funcTH(void *arg)
{
    srand(time(NULL));
    /* basliyoo */
    struct ingredients temp = *((struct ingredients *)arg);

    switch (temp.i)
    {
        case 1: fprintf(stderr,"chef%d is waiting milk and sugar\n",temp.i);
            break;
        case 2: fprintf(stderr,"chef%d is waiting milk and walnuts\n",temp.i);
            break;
        case 3: fprintf(stderr,"chef%d is waiting sugar and walnuts\n",temp.i);
            break;
        case 4:fprintf(stderr,"chef%d is waiting milk and flour\n",temp.i);
            break;
        case 5: fprintf(stderr,"chef%d is waiting sugar and flour\n",temp.i);
            break;
        case 6: fprintf(stderr,"chef%d is waiting walnuts and flour\n",temp.i);
            break;
        default:
            break;
    }

    while (lcv == 1)
    {
        if (temp.w == 1 && temp.s == 1)
        {
            if (temp.ingp[0] == 1 && temp.ingp[3] == 1)
            {
               // fprintf(stderr,"chef%d is waiting flour and milk\n",temp.i);
                sem_wait(&mutex);

                fprintf(stderr,"the wholesaler delivers milk and flour\n");
                fprintf(stderr, "chef%d has taken the milk\n", temp.i);
                fprintf(stderr, "the wholesaler is waiting for the dessert\n");
                fprintf(stderr, "chef%d has taken the flour\n", temp.i);

                temp.ingp[0] = 0;
                temp.ingp[1] = 0;
                temp.ingp[2] = 0;
                temp.ingp[3] = 0;
                int result = (rand() % 5) + 1;
                fprintf(stderr, "chef%d is preparing the dessert\n", temp.i);
                sleep(result);

                sem_post(&mainmutex);
                sem_post(&mutex);
                fprintf(stderr, "chef%d has delivered the dessert to the wholesaler\n", temp.i);
                fprintf(stderr,"the wholesaler has obtained the dessert and left to sell it\n");
                fprintf(stderr,"chef%d is waiting milk and flour\n",temp.i);
            }
        }
        else if (temp.w == 1 && temp.f == 1)
        {
            if (temp.ingp[2] == 1 && temp.ingp[3] == 1)
            {
                sem_wait(&mutex);

                fprintf(stderr,"the wholesaler delivers milk and sugar\n");
                fprintf(stderr, "chef%d has taken the milk\n", temp.i);
                fprintf(stderr, "the wholesaler is waiting for the dessert\n");
                fprintf(stderr, "chef%d has taken the sugar\n", temp.i);

                temp.ingp[0] = 0;
                temp.ingp[1] = 0;
                temp.ingp[2] = 0;
                temp.ingp[3] = 0;
                int result = (rand() % 5) + 1;
                fprintf(stderr, "chef%d is preparing the dessert \n", temp.i);
                sleep(result);
                sem_post(&mainmutex);
                sem_post(&mutex);
                fprintf(stderr, "chef%d has delivered the dessert to the wholesaler\n", temp.i);
                fprintf(stderr,"the wholesaler has obtained the dessert and left to sell it\n");
                fprintf(stderr,"chef%d is waiting milk and sugar\n",temp.i);
            }
        }

        else if (temp.w == 1 && temp.m == 1)
        {
            if (temp.ingp[0] == 1 && temp.ingp[2] == 1)
            {
                sem_wait(&mutex);

                fprintf(stderr,"the wholesaler delivers sugar and flour\n");
                fprintf(stderr, "chef%d has taken the sugar\n", temp.i);
                fprintf(stderr, "the wholesaler is waiting for the dessert\n");
                fprintf(stderr, "chef%d has taken the flour\n", temp.i);

                temp.ingp[0] = 0;
                temp.ingp[1] = 0;
                temp.ingp[2] = 0;
                temp.ingp[3] = 0;
                int result = (rand() % 5) + 1;
                fprintf(stderr, "chef%d is preparing the dessert\n", temp.i);
                sleep(result);
                sem_post(&mainmutex);
                sem_post(&mutex);
                fprintf(stderr, "chef%d has delivered the dessert to the wholesaler\n", temp.i);
                fprintf(stderr,"the wholesaler has obtained the dessert and left to sell it\n");
                fprintf(stderr,"chef%d is waiting sugar and flour\n",temp.i);
            }
        }

        else if (temp.s == 1 && temp.f == 1)
        {
            if (temp.ingp[1] == 1 && temp.ingp[3] == 1)
            {
                sem_wait(&mutex);

                fprintf(stderr,"the wholesaler delivers walnuts and milk\n");
                fprintf(stderr, "chef%d has taken the walnuts\n", temp.i);
                fprintf(stderr, "the wholesaler is waiting for the dessert\n");
                fprintf(stderr, "chef%d has taken the milk\n", temp.i);

                temp.ingp[0] = 0;
                temp.ingp[1] = 0;
                temp.ingp[2] = 0;
                temp.ingp[3] = 0;
                int result = (rand() % 5) + 1;
                fprintf(stderr, "chef%d is preparing the dessert\n", temp.i);
                sleep(result);
                sem_post(&mainmutex);
                sem_post(&mutex);
                fprintf(stderr, "chef%d has delivered the dessert to the wholesaler\n", temp.i);
                fprintf(stderr,"the wholesaler has obtained the dessert and left to sell it\n");
                fprintf(stderr,"chef%d is waiting milk and walnuts\n",temp.i);

            }
        }

        else if (temp.s == 1 && temp.m == 1)
        {
            if (temp.ingp[0] == 1 && temp.ingp[1] == 1)
            {
                sem_wait(&mutex);
                
                fprintf(stderr,"the wholesaler delivers flour and walnuts\n");
                fprintf(stderr, "chef%d has taken the walnuts\n", temp.i);
                fprintf(stderr, "the wholesaler is waiting for the dessert\n");
                fprintf(stderr, "chef%d has taken the flour\n", temp.i);

                temp.ingp[0] = 0;
                temp.ingp[1] = 0;
                temp.ingp[2] = 0;
                temp.ingp[3] = 0;
                int result = (rand() % 5) + 1;
                fprintf(stderr, "chef%d is preparing the dessert\n", temp.i);
                sleep(result);
                sem_post(&mainmutex);
                sem_post(&mutex);
                fprintf(stderr, "chef%d has delivered the dessert to the wholesaler\n", temp.i);
                fprintf(stderr,"the wholesaler has obtained the dessert and left to sell it\n");
                fprintf(stderr,"chef%d is waiting walnuts and flour\n",temp.i);
            }
        }

        else if (temp.f == 1 && temp.m == 1)
        {
            if (temp.ingp[1] == 1 && temp.ingp[2] == 1)
            {
                sem_wait(&mutex);

                fprintf(stderr,"the wholesaler delivers sugar and walnuts\n");
                fprintf(stderr, "chef%d has taken the walnuts\n", temp.i);
                fprintf(stderr, "the wholesaler is waiting for the dessert\n");
                fprintf(stderr, "chef%d has taken the sugar\n", temp.i);

                temp.ingp[0] = 0;
                temp.ingp[1] = 0;
                temp.ingp[2] = 0;
                temp.ingp[3] = 0;
                int result = (rand() % 5) + 1; //(1 to 5 inclusive)
                fprintf(stderr, "chef%d is preparing the dessert \n", temp.i);
                sleep(result);
                sem_post(&mainmutex);
                sem_post(&mutex);
                fprintf(stderr, "chef%d has delivered the dessert to the wholesaler\n", temp.i);
                fprintf(stderr,"the wholesaler has obtained the dessert and left to sell it\n");
                fprintf(stderr,"chef%d is waiting sugar and walnuts\n",temp.i);

            }
        }
    }

    pthread_exit(NULL);
}
