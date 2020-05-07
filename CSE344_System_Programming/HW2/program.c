#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>


#define NUMBEROFXY 10

double sumOfValues(int[]);
int loopSize(char* );
int writeToFile(int[], int[],double[],double[],int);
int writeToOutput(char* outputPath, int x[], int y[], double m,double b,double mae, double mse, double rmse);
double calculateMSE(int x[], int y[]);
double calculateMAE(int x[], int y[]);
double getLSM(char *param);
double printP2Finish(double[],double[], double[] ,int  );


int inputSize = 0;
int spipe = 0;
int usr1ForParent = 0, usr1ForChild = 0;
int usr2ForParent = 0, usr2ForChild = 0;

int readLine = 0;

/*************************************************************************************/
void catcherForChild(int signum)
{
    switch (signum)
    {
    case SIGUSR1:
        usr2ForChild = 1;
        break;

    case SIGUSR2:
        readLine += 1;
        usr1ForChild = 1;
        break;
    case SIGPIPE:
        spipe = 1;
        break;
    }
}

void catcherForParent(int signum)
{
    switch (signum)
    {
    case SIGUSR1:
        usr2ForParent = 1;
        break;

    case SIGUSR2:
        usr1ForParent = 0;
        break;
    }
}

int fdWrite;
int main(int argc, char **argv)
{
    int opt;
    char *inputPath, *outputPath;
    if(argc < 4) {
        printf("USAGE : ./program -i inputPath -o outputPath\n");
        return -1;
    }

    while ((opt = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (opt)
        {
        case 'o':
            outputPath = optarg;
            break;
        case 'i':
            inputPath = optarg;
            break;
        case ':':
            printf("Option needs a value %s\n", optarg);
            printf("USAGE : ./program -i inputPath -o outputPath\n");
            return -1;
        case '?':
            printf("Unknown option: %c\n", optopt);
            printf("USAGE : ./program -i inputPath -o outputPath\n");
            return -1;
        }
    }
    for (; optind < argc; optind++)
    {
        printf("Extra arguments: %s\n", argv[optind]);
        printf("USAGE : ./program -i inputPath -o outputPath\n");
        return -1;
    }
    if (strcmp(outputPath, inputPath) == 0)
    {
        printf("Outputpath and inputPath cannot be the same. \n");
        return -1;
    }

    int fdRead;
    struct flock lock;

    struct sigaction sactForChild, sactForParent;

    sigset_t signalmask;

    sigset_t sigset, sigset2;
    sigemptyset(&sactForChild.sa_mask);
    sactForChild.sa_flags = 0;
    sactForChild.sa_handler = catcherForChild;

    sigemptyset(&sactForParent.sa_mask);
    sactForParent.sa_flags = 0;
    sactForParent.sa_handler = catcherForParent;

    //sinyale atama yap
    if ((sigemptyset(&signalmask) == -1) || (sigaddset(&signalmask, SIGINT) == -1) || (sigaddset(&signalmask, SIGSTOP) == -1))
    {
        perror("Failed to initialize the signal mask");
        return 1;
    }
    //kontrol et output var mı ?
    int o = open(outputPath,O_WRONLY);
    if(o == -1) {
        perror("Failed output file error \n");
        return -1;
    } else {
        close(o);
    }

    //temp dosyasını oluştur..
    char filename[] = "tempfile-XXXXXXX";
    
    if ((fdWrite = mkstemp(filename)) == -1) {
        perror("Failed temp file error \n");
        return -1;
    }
    memset(&lock, 0, sizeof(lock));

    pid_t PID = fork();

    /************* CHILD (P2) ******************/
    
    if (PID == 0)
    {
        double mae[1000];
        double mse[1000];
        double rmse[1000];
        int tut = 0;
        while ((usr2ForChild != 1 || readLine != 0) && spipe == 0)
        {

            signal(SIGUSR2, catcherForChild);
            signal(SIGPIPE, catcherForChild);
            if (usr1ForChild == 0 && spipe == 0)
            {
                sigemptyset(&sigset);
                //sigdelset(&sigset, SIGUSR2);
                sigsuspend(&sigset);
            }
            usr1ForChild = 0;

            while (readLine != 0)
            {

                //temp kitliyse, sinyal bekle-- temp kitli değilse kitle
                lock.l_type = F_WRLCK;
                if (fcntl(fdWrite, F_SETLK, &lock) == -1) //-1 ise kitlii..
                { 
                    sigfillset(&sigset);
                    sigdelset(&sigset, SIGUSR2);

                    sigsuspend(&sigset);
                }

/*********************************** OKU ********************************************************************/
                int x[10], y[10]; 
                char sayi[100];
                
                int fd = open(filename, O_RDONLY);
                if (fd == -1)
                {
                    perror("Error :");
                    return -1;
                }
                char buf[1];
                int satir = 0;
                int xignore = 0;
                int yignore = 0;
                char mchar[100];
                int XorY = 0;
                int numOrder = 0;
                int i = 0;
                int ix = 0;
                int iy = 0;
                double bSayisi = 0;
                double mSayisi = 0;
                while (read(fd, buf, sizeof(buf)) > 0)
                {
                    if (numOrder < 20)
                    {
                        if (buf[0] >= 48 && buf[0] <= 57)
                        {
                            sayi[i] = buf[0];
                            i++;
                        }
                        else if (i != 0)
                        {
                            sayi[i] = '\0';
                            if (XorY % 2 == 0)
                            {
                                x[ix] = atoi(sayi);
                                ix++;
                            }
                            else
                            {
                                y[iy] = atoi(sayi);
                                iy++;
                            }
                            i = 0;
                            numOrder++;
                            XorY++;
                        }
                    }
                    else if (xignore < 3)
                    {
                        xignore++;
                        continue;
                    }
                    else if (xignore == 3)
                    {
                        if (buf[0] != 'x')
                        {
                            mchar[i] = buf[0];
                            i++;
                        }
                        else if (buf[0] == 'x')
                        {
                            mchar[i] = '\0';
                            i = 0;
                            xignore++;
                            mSayisi = getLSM(mchar);
                        }
                    }
                    else if (yignore < 3)
                    {
                        yignore++;
                        continue;
                    }
                    else if (yignore == 3)
                    {
                        if (buf[0] == ' ' || buf[0] == '\n')
                        {
                            yignore++;
                        }
                        mchar[i] = buf[0];
                        i++;
                    }
                }
                close(fd);
                mchar[i] = '\0';

                bSayisi = getLSM(mchar);      
/********************************************************************************************************/
                //okuduğun satırı sil, boşluk yaz tempe..
                readLine--;

                //tempin kilidini aç
                lock.l_type = F_UNLCK;
                fcntl(fdWrite, F_SETLK, &lock);

                //parenta USR1 sinyali gönder //yazsın

                //sinyale atama yap sonra kritik bölge aç
                if (sigprocmask(SIG_BLOCK, &signalmask, NULL) == -1)
                    break;
               
                //hesaplama yap
                double MAE = calculateMAE(x,y);
                double MSE = calculateMSE(x,y);
                double RMSE = sqrt(MSE);

                mae[tut] = MAE;
                mse[tut] = MSE;
                rmse[tut] = RMSE;
                tut++;
                //sinyal için kritik bölge kapa
                if (sigprocmask(SIG_UNBLOCK, &signalmask, NULL) == -1)
                    break;

                //outputa yaz
                writeToOutput(outputPath,x,y,mSayisi,bSayisi,MAE,MSE,RMSE);
            }
            kill(getppid(), SIGUSR2);
        }

        kill(getppid(), SIGUSR1);
        printP2Finish(mae,mse,rmse,tut);
        
        close(fdWrite);
        unlink(filename); //temp
        exit(0);
    }

    //inputPath (10tane x,y olarak okunacak !) ekrana bas
    fdRead = open(inputPath, O_RDONLY);
    if (fdRead == -1)
    {
        fprintf(stderr, "Inputpath  Error: ");
        return -1;
    }

    ssize_t bytes_read;
    unsigned char buf[1];
    bytes_read = read(fdRead, buf, sizeof(buf));

    int i, k ,l=0, j = 0, index = 0;
    int x[10], y[10], x2[10], xy[10];
    

    //kaç tane 20lik okunacak ?
    inputSize = loopSize(inputPath);
    double m[inputSize], b[inputSize];
    //inputSize kaç tane 20lik varsa o kadar döner..
    while (l < inputSize) {

        /************* PARENT (P1) **************/
        signal(SIGUSR1, catcherForParent);
        signal(SIGUSR2, catcherForParent);
        signal(SIGPIPE, catcherForParent);
        if (usr1ForParent != 0) {

            sigfillset(&sigset);
            sigdelset(&sigset, SIGUSR2);

            sigsuspend(&sigset);
        }
        usr1ForParent = 1;
        for (k = 0; k < 2*NUMBEROFXY; k++) {
            //1 harf 1 harf...
            for (i = 0; i < 1; i++)  {
                int ascii = buf[i];
                unsigned int *num;
                int ret = -1;
                num = (int *)&ascii;

                memset(buf, 0, sizeof(buf));
                if (j % 2 == 0) {
                    x[index] = *num;
                    x2[index] = x[index] * x[index];

                } else {
                    y[index] = *num;
                    xy[index] = x[index] * y[index];
                   // printf("x: %d - y: %d !! x2 :%d - xy: %d\n",x[index], y[index], x2[index],xy[index]);
                    index++;
                }
            }
            bytes_read = read(fdRead, buf, sizeof(buf));
            j++;
        }

        //sinyale atama yap sonra kritik bölge aç
        if (sigprocmask(SIG_BLOCK, &signalmask, NULL) == -1)
            break;

        //hesaplama yap
        double sumX = sumOfValues(x);
        double sumY = sumOfValues(y);
        double sumXY = sumOfValues(xy);
        double sumX2 = sumOfValues(x2);

        m[l] = (NUMBEROFXY * sumXY - sumX * sumY) / (NUMBEROFXY * sumX2 - sumX * sumX);
        b[l] = (sumY - m[l-1] * sumX) / NUMBEROFXY;
        l++;

       // fprintf(stderr, "x:%f - y:%f - x2:%f - xy:%f - m:%f - b:%f\n", sumOfValues(x), sumOfValues(y), sumOfValues(x2), sumOfValues(xy), m[l-1], b[l-1]);

        //sinyal için kritik bölge kapa
        if (sigprocmask(SIG_UNBLOCK, &signalmask, NULL) == -1)
            break;

        //temp kitliyse, usr1 bekle-- temp kitli değilse kitle
        lock.l_type = F_WRLCK; //bak buna!!!
        if (fcntl(fdWrite, F_SETLK, &lock) == -1) { //-1 ise kitlii..
            //usr1 bekle..
            sigfillset(&sigset);
            sigdelset(&sigset, SIGUSR1);

            sigsuspend(&sigset);
        }

        //yaz
        writeToFile(x,y,m,b,l);


        //templin kilidini aç
        lock.l_type = F_UNLCK;
        fcntl(fdWrite, F_SETLK, &lock);

        //childa USR1 sinyali gönder //okusun
        kill(PID, SIGUSR2);
    }
    kill(PID, SIGPIPE);
    signal(SIGUSR1, catcherForParent);
    signal(SIGUSR2, catcherForParent);
    signal(SIGPIPE, catcherForParent);
    //input kapa
    close(fdRead);

    //childa USR2 sinyalini gönder //işim bitti

    //ekrana basılacaklar
    fprintf(stderr,"P1 : LINE NUMBER : %d\n",inputSize);
    fprintf(stderr,"P1 : READING BYTE : %d\n",inputSize*20);

    //suspend usr2 bekle
    if (usr2ForParent != 1)
    {
        sigfillset(&sigset2);
        sigdelset(&sigset2, SIGUSR1);

        sigsuspend(&sigset2);

        //usr2ForParent = 0;
    }

     //inputu sil
    unlink(inputPath);

   // close(fdWrite);
  //  unlink(filename); //temp
    raise(SIGTERM);
    exit(0);
}
double printP2Finish(double mae[],double mse[], double rmse[], int size ){
    int i;
    for (i = 0; i < size; i++) {
        fprintf(stderr,"MAE : %.3f\t",mae[i]);
        fprintf(stderr,"MSE : %.3f\t",mse[i]);
        fprintf(stderr,"RMSE : %.3f\n",rmse[i]);
    }
}
double sumOfValues(int arr[])
{
    int result = 0, i;
    for (i = 0; i < NUMBEROFXY; i++)
    {
        result += arr[i];
    }
    return result;
}

int loopSize(char* inputPath) {
    int fdRead;
    fdRead = open(inputPath, O_RDONLY);
    if (fdRead == -1)
    {
        perror("Inputpath Error: ");
        return -1;
    }

    ssize_t bytes_read;
    unsigned char buf[1];
    bytes_read = read(fdRead, buf, sizeof(buf));
    
    int a = 0;
    while (bytes_read == sizeof(buf))
    {  
        bytes_read = read(fdRead, buf, sizeof(buf));
        a++;
    }
    return a/20;
}
int writeToOutput(char* outputPath, int x[], int y[], double m,double b, double mae, double mse, double rmse){
  
    int fd = open(outputPath, O_RDWR | O_APPEND);
    if (fd == -1)  {
        perror("Output File Error: ");
        return -1;
    }
    int i;
    for (i = 0; i < NUMBEROFXY; i++) {

        int asciiX = x[i];
        int asciiY = y[i];
        int *numX, *numY;
        int ret = -1;
        numX = (int *)&asciiX;
        numY = (int *)&asciiY;
        char bufX[4] = {0}; //to use with write()
        char bufY[4] = {0};
        memset(bufX, 0, sizeof(bufX));
        snprintf(bufX, sizeof *numX, "%d", *numX); //print to buffer
        memset(bufY, 0, sizeof(bufY));
        snprintf(bufY, sizeof *numY, "%d", *numY);
        write(fd,"(",1);
        write(fd, bufX, strlen(bufX));
        write(fd,", ",2);
        write(fd, bufY, strlen(bufY));
        write(fd,"),",2);        
    }

    double asciiM = m;
    double asciiB = b;
    double *numM =(double *)&asciiM;
    double *numB =(double *)&asciiB;
    char bufM[16] = {0}; //to use with write()
    char bufB[16] = {0};
    memset(bufM, 0, sizeof(bufM));
    snprintf(bufM, sizeof *numM, "%.3f", *numM); //print to buffer
    memset(bufB, 0, sizeof(bufB));
    snprintf(bufB, sizeof *numB, "%.3f", *numB);
    write(fd," ",1);
    write(fd, bufM, strlen(bufM));
    write(fd,"x + ",4);
    write(fd, bufB, strlen(bufB)); 
    write(fd,", ",2);


    double asciiMae = mae;
    double asciiMse = mse;
    double asciiRmse = rmse;

    double *numMae =(double *)&asciiMae;
    double *numMse =(double *)&asciiMse;
    double *numRmse =(double *)&asciiRmse;

    char bufA[16] = {0}; //to use with write()
    char bufS[16] = {0};
    char bufMs[16] = {0};

    memset(bufA, 0, sizeof(bufA));
    snprintf(bufA, sizeof *numMae, "%.3f", *numMae); //print to buffer
    
    memset(bufS, 0, sizeof(bufS));
    snprintf(bufS, sizeof *numMse, "%.3f", *numMse);
    
    memset(bufMs, 0, sizeof(bufMs));
    snprintf(bufMs, sizeof *numRmse, "%.3f", *numRmse);
   
    write(fd," ",1);
    write(fd, bufA, strlen(bufA));
    write(fd,", ",2);
    write(fd, bufS, strlen(bufS)); 
    write(fd,", ",2);
    write(fd, bufMs, strlen(bufMs)); 
    write(fd,"\n",1);


    close(fd);
    return 0;
}
int writeToFile(int x[], int y[], double m[],double b[],int index){
    lseek(fdWrite, 0 , SEEK_SET);
    int i;
    for (i = 10*(index-1); i < NUMBEROFXY*index; i++) {

        int asciiX = x[i];
        int asciiY = y[i];
        int *numX, *numY;
        int ret = -1;
        numX = (int *)&asciiX;
        numY = (int *)&asciiY;
        char bufX[4] = {0}; //to use with write()
        char bufY[4] = {0};
        memset(bufX, 0, sizeof(bufX));
        snprintf(bufX, sizeof *numX, "%d", *numX); //print to buffer
        memset(bufY, 0, sizeof(bufY));
        snprintf(bufY, sizeof *numY, "%d", *numY);
        write(fdWrite,"(",1);
        write(fdWrite, bufX, strlen(bufX));
        write(fdWrite,", ",2);
        write(fdWrite, bufY, strlen(bufY));
        write(fdWrite,"),",2);        
    }

    double asciiM = m[index-1];
    double asciiB = b[index-1];
    double *numM =(double *)&asciiM;
    double *numB =(double *)&asciiB;
    char bufM[16] = {0}; //to use with write()
    char bufB[16] = {0};
    memset(bufM, 0, sizeof(bufM));
    snprintf(bufM, sizeof *numM, "%.3f", *numM); //print to buffer
    memset(bufB, 0, sizeof(bufB));
    snprintf(bufB, sizeof *numB, "%.3f", *numB);
    write(fdWrite," ",1);
    write(fdWrite, bufM, strlen(bufM));
    write(fdWrite,"x + ",4);
    write(fdWrite, bufB, strlen(bufB)); 
    write(fdWrite,"\n",1);

    return 0;
}

double calculateMAE(int x[], int y[]) {
    double MAE ;
    double dist = 0;
    for (int i = 0; i < 10; i++) {
        double r = y[i] - x[i];
        if( r < 0 )
            r *=-1;
        dist += r;
    }
    MAE = dist /10;
    return MAE ;
}
double calculateMSE(int x[], int y[]) {
    double MSE ;
    double dist = 0;
    for (int i = 0; i < 10; i++) {
        double r = y[i] - x[i];
        dist += r*r;
    }
    MSE = dist / 10;
    return MSE ;
}
double getLSM(char *param)
{
    int doubleNumber;
    double ddoubleNumber;
    int j = 0;
    int i = 0;
    char sayi[100];
    int checkdot = 0;
    double bol = 0;
    while (param[j] != '\0')
    {
        if (param[j] >= 48 && param[j] <= 57)
        {
            sayi[i] = param[j];
            i++;
            if (checkdot == 1)
                bol++;
        }
        else if (param[j] == '.')
        {
            checkdot = 1;
        }
        j++;
    }
    sayi[i] = '\0';
    doubleNumber = atoi(sayi);
    ddoubleNumber = doubleNumber;

    double bolum = 1;
    while (bol > 0)
    {
        bolum *= 10;
        bol--;
    }

    return (ddoubleNumber / bolum);
}