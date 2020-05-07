#include "source.h"

struct Struct{  
        int length;
        int A[30][60];
        int B[64][30];
        int corner;
};
struct resultStruct{
        int C[30][30];
        int corner;
};

struct Struct object;
int parent;
int N;

void childExit (int signo) {
    childCount++;
}

void startChild(int signo){
    wakeUp = 1;
}

void exitProgram(int signo) {
    kill(parent,SIGTERM);
}

int main(int argc, char *argv[])
{
    //girdi kontrol..
    int opt;
    char *inputPathA, *inputPathB;
    
    while ((opt = getopt(argc, argv, ":i:j:n:")) != -1)
    {
      switch (opt)
      {
      case 'i':
         inputPathA = optarg;
         break;
      case 'n':
         N = atoi(optarg);
         break;
      case 'j':
         inputPathB = optarg;
         break;
      case ':':
         fprintf(stderr,"Option needs a value %s\n", optarg);
         fprintf(stderr,"USAGE : ./program -i inputPathA -j inputPathB -n 8\n");
         return -1;
      case '?':
         fprintf(stderr,"Unknown option: %c\n", optopt);
         fprintf(stderr,"USAGE : ./program -i inputPathA -j inputPathB -n 8\n");
         return -1;
      }
    }
    if(optind < 7) {
        fprintf(stderr,"USAGE : ./program -i inputPathA -j inputPathB -n 8\n");
        return -1;
    }
    if(N < 0 || N > 4) {
        fprintf(stderr,"Value Ranges of n is [1,4]\n");
        return -1;
    }
    for (; optind < argc; optind++)
    {
        fprintf(stderr,"Extra arguments: %s\n", argv[optind]);
        fprintf(stderr,"USAGE : ./program -i inputPathA -j inputPathB -n 8\n");
        return -1;
    }
    /*********************************************/

    sigset_t sigset;
    int pidArr[4];
	int p[2], pid = 1, nbytes; 

	if (pipe(p) < 0) 
		exit(1); 
    
    signal(SIGUSR2,startChild);  //çocukları uyandırmak için
    signal(SIGINT,exitProgram); //ctr-c için
    int i;
    for (i = 0; i < 4; i++)
    {                               
        if(pid > 0 ) {
            pid = fork();
            pidArr[i] = pid;
        }
    }
    
    int w; //write loop
	if (pid  > 0) {

        /*************** OKUMA *******************/  
        const int length = exponential(2,N);

        // read inputPathA
        int fdA = open(inputPathA, O_RDONLY);
        if (fdA == -1)
        {
            fprintf(stderr, "InputpathA  Error ! ");
            return -1;
        }

        const int size =  length * length ; // (2^n)x(2^n) kadar..

        if( inputPathSize(inputPathA) < size ) {
            perror("Not enough element on inputPathA\n");
            close(fdA);
            return -1;
        }

        int x=0, y=0;
        ssize_t bytes_read;
        unsigned char buf[1];
        int i,j,k;
        int A[length][length];
        bytes_read = read(fdA, buf, sizeof(buf));
        for (i = 0; i < size ; i++) {

            //1 harf 1 harf...
            for (j = 0; j < 1; j++)  {
                int ascii = buf[j];
                unsigned int *num;
                int ret = -1;
                num = (int *)&ascii;

                A[x][y] = *num;
                y++;
            }

            if ( y == length) {
                x++;
                y=0;
            }
            bytes_read = read(fdA, buf, sizeof(buf));
        }
        close(fdA); 
      
        // read inputPathB
        int fdB = open(inputPathB, O_RDONLY);
        if (fdB == -1) {
            fprintf(stderr, "InputpathB  Error ! ");
            return -1;
        }

        if( inputPathSize(inputPathB) < size ) {
            perror("Not enough element on inputPathB\n");
            close(fdB);
            return -1;
        }

        x=0, y=0;
        bytes_read = read(fdB, buf, sizeof(buf));
        int B[length][length];
        for (i = 0; i < size ; i++) {

            //1 harf 1 harf...
            for (j = 0; j < 1; j++)  {
                int ascii = buf[j];
                unsigned int *num;
                int ret = -1;
                num = (int *)&ascii;

                B[x][y] = *num;
                y++;
            }

            if ( y == length ) {
                x++;
                y=0;
            }
            bytes_read = read(fdB, buf, sizeof(buf));
        }
        close(fdB);
    
        object.corner = 1;
        object.length = length;
        
        struct resultStruct corners[4];
        int cIndex = 0;

        /********************************************/

        parent = getpid();
        signal(SIGCHLD,childExit);
        for (w = 0; w < 4; w++)
        {
            int iA,jB;
            int borderA,borderB;
            if( object.corner < 3) { //1,2
                iA = 0;
                borderA = length/2;
                if( object.corner == 1) {
                    jB = 0;
                    borderB = length/2;
                }
                else {
                    jB = length/2;
                    borderB = length;
                }
            }

            else{ //3,4
                iA = length/2;
                borderA = length;
                if( object.corner == 3) {
                    jB = 0;
                    borderB = length/2;
                }
                else {
                    jB = length/2;
                    borderB = length;
                }        
            }
            //Anın  her köşesi için
            int x = 0, y = 0;
            for ( i = iA; i < borderA; i++,x++)
            {
                for (j = 0, y = 0; j < length; j++,y++)
                {
                    object.A[x][y] = A[i][j];
                }
            }
            //Bnın  her köşesi için
            x = 0, y = 0;
            for ( i = 0; i < length; i++,x++)
            {
                for (j = jB, y = 0; j < borderB; j++,y++)
                {
                    object.B[x][y] = B[i][j];
                }
            }
            write(p[1],&object,sizeof(object));
            object.corner++;
        }

        for (i = 0; i < 4; i++)
        {
            kill(pidArr[i],SIGUSR2);
        }
     
        while ( (wpid = wait(&status) ) > 0); //wait all the childs

        struct resultStruct end;
        for (i = 0; i < 4; i++)
        {
            read(p[0],&end,sizeof(end));
            corners[cIndex] = end;
            cIndex++;
        }

        close(p[0]);
        close(p[1]);

		wait(NULL);

        double C[length][length];

        //collect all corners
        int c1,c2,c3,c4;
        for ( x = length/2, i = 0; x < length, i < length/2; i++, x++)
        {
            for ( y = length/2, j = 0; y < length, j < length/2; j++, y++)
            {
                for (k = 0; k < 4; k++)
                { 
                    if(corners[k].corner == 1)
                        c1 = k;
                    else if(corners[k].corner == 2)
                        c2 = k;
                    else if(corners[k].corner == 3)
                        c3 = k;
                    else
                        c4 = k;
                }
                C[i][j] = corners[c1].C[i][j]; //1. köşe
                C[x][j] = corners[c3].C[i][j]; //3. köşe 
                C[i][y] = corners[c2].C[i][j]; //2. köşe
                C[x][y] = corners[c4].C[i][j]; //4. köşe
            }
        }
        /*
        //print C screen
        for(i=0;i<length;i++){
            for(j=0;j<length;j++)
                printf("%.1f ",C[i][j]);
            printf("\n");
        }
        printf("\n\n\n");
        */
        /************  Singular Values Of the C  ***************/
        dummy_array = (double*) malloc(length * sizeof(double));
        if (dummy_array == NULL) {fprintf(stderr,"No memory available\n"); exit(-1); }
        err = Singular_Value_Decomposition((double*) C, length, length, (double*) U,singular_values, (double*) V, dummy_array);
        free(dummy_array);

        if (err < 0) fprintf(stderr,"Failed to converge\n");
        else {
            for (int i = 0; i < length; i++)
            {
                fprintf(stderr,"%.3f ",singular_values[i]);
            }        
        }
        fprintf(stderr,"\n");

        /****************************/
        exit(0);
    } 

	else { 
        
        if(wakeUp == 0) {
            sigemptyset(&sigset);
            sigsuspend(&sigset);
        }
        
        struct Struct st; // A ve B'yi al
        struct resultStruct result;  //C'yi gönder

        read(p[0],&st,sizeof(st));

        //Hesapla
        int i,j,k;
        for(i = 0; i < st.length/2; i++)
        {
            result.corner = st.corner;
            for(j = 0; j < st.length/2; j++)
            {
                for(k = 0; k < st.length; k++)
                {
                    result.C[i][j] += st.A[i][k] * st.B[k][j];
                }
            }
        }

        write(p[1],&result,sizeof(result)); 
                
        exit(0); 
	}

	return 0; 
}

int exponential(int num, int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++) {
        result *= num;
    }
    return result;
}

int inputPathSize(char* inputPath) {
    int fdRead;
    fdRead = open(inputPath, O_RDONLY);
    if (fdRead == -1)
    {
        perror("Inputpath Error !");
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
    close(fdRead);
    return a;
}