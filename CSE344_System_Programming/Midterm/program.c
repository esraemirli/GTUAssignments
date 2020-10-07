#include <stdio.h>     /* standard I/O routines.               */
#include <sys/types.h> /* various type definitions.            */
#include <sys/ipc.h>   /* general SysV IPC structures          */
#include <sys/shm.h>   /* semaphore functions and structs.     */
#include <sys/sem.h>   /* shared memory functions and structs. */
#include <unistd.h>    /* fork(), etc.                         */
#include <wait.h>      /* wait(), etc.                         */
#include <time.h>      /* nanosleep(), etc.                    */
#include <stdlib.h>    /* rand(), etc.                         */

#include <errno.h>     /* errno, ECHILD            */
#include <fcntl.h>     /* O_CREAT, O_EXEC          */
#include <semaphore.h> /* sem_open(), sem_destroy(), sem_wait().. */
#include <pthread.h>
#include <signal.h>

int whichActor(int *arr, int pid, int size);
typedef struct Kitchen
{
    sem_t mutex_kitchen;
    int soup;
    int main_course;
    int desert;
    int capacity;

} Kitchen;
Kitchen *kitchen_addr;

typedef struct Counter
{

    sem_t sem_desert, sem_main_course, sem_soup;

    sem_t mutex_counter;
    sem_t sem_table; //table icin sh olustutulmadi, burda dursun
    sem_t sem_cook;  //tezgah doluluğu kadar yemek koysun diye
    int soup;
    int main_course;
    int desert;
    int capacity;
    int studentNum;
    int tableNum;

} Counter;
Counter *counter_addr;

typedef struct Actors
{
    sem_t sem_cook;
    sem_t sem_supplier;
    sem_t sem_student;

    int supplier_finish;
    int flagSup; //sup bitti mi ?

    int *cooks;
    int *students;
    int supplier;

} Actors;
Actors *actor_addr;
void exitProgram(Actors* actor_addr, Kitchen* kitchen_addr, Counter* counter_addr);

int studentSize , cookSize , tableSize ; 
int counterSize , kitchenSize , rounds ;

int readIndex = 0;
int main(int argc, char *argv[])
{

    //girdi kontrol..
    int opt;
    char *filePath;
    while ((opt = getopt(argc, argv, "N:M:T:S:L:F:")) != -1)
    {
        switch (opt)
        {
            case 'N':
                cookSize = atoi(optarg);
                break;
            case 'M':
                studentSize = atoi(optarg);
                break;
            case 'T':
                tableSize = atoi(optarg);
                break;
            case 'S':
                counterSize = atoi(optarg);
                break;
            case 'L':
                rounds = atoi(optarg);
                break;
            case 'F':
                filePath = optarg;
                break;
            case ':':
                fprintf(stderr,"Option needs a value %s\n", optarg);
                fprintf(stderr,"Example: $./program -N 3 -M 12 -T 5 -S 4 -L 13 -F filePath\n");
                return -1;
            case '?':
                fprintf(stderr,"Unknown option: %c\n", optopt);
                fprintf(stderr,"Example: $./program -N 3 -M 12 -T 5 -S 4 -L 13 -F filePath\n");
                return -1;
        }
    }
    if(optind < 13) {
        fprintf(stderr,"Example: $./program -N 3 -M 12 -T 5 -S 4 -L 13 -F filePath\n");
        return -1;
    }
    for (; optind < argc; optind++)
    {
        fprintf(stderr,"Extra arguments: %s\n", argv[optind]);
        fprintf(stderr,"Example: $./program -N 3 -M 12 -T 5 -S 4 -L 13 -F filePath\n");
        return -1;
    }
    if(cookSize <= 2 ){
        fprintf(stderr,"Rules : N > 2 \n");
        return -1;
    }
    if(studentSize <= cookSize){
        fprintf(stderr,"Rules : M > N \n");
        return -1;
    }
    if(counterSize <= 3){
        fprintf(stderr,"Rules : S > 3 \n");
        return -1;
    }
    if( studentSize <= tableSize ) { 
        fprintf(stderr,"Rules : M > T \n");
        return -1;
    }
    if(tableSize < 1) {
        fprintf(stderr,"Rules : T >= 1 \n");
        return -1;
    }
    if(rounds < 3) {
        fprintf(stderr,"Rules : L >= 3 \n");
        return -1;
    }
    kitchenSize = 2*rounds*studentSize + 1;
    /*********************************************/
    // read inputPathA
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        perror("FilePath  Error ! ");
        return -1;
    }


    int retval;
    int shm_actorID, shm_cook, shm_student;
    /* Make actors shared memory segment . */
    shm_actorID = shmget(IPC_PRIVATE, sizeof(Actors), IPC_CREAT | 0644);
    if (shm_actorID == -1)
        perror(" Creation Actors ");
    /* Attach actors the segment . */
    actor_addr = (Actors *)shmat(shm_actorID, NULL, 0);
    /* Initialize the semaphore . 2. parametre 0 ise thread olarak, 0'dan farklıysa process olarak işlem yapıyorsun..*/
    retval = sem_init(&actor_addr->sem_cook, 1, 0);
    if (retval != 0)
    {
        perror(" Couldn ’t initialize. ");
        exit(3);
    }
    retval = sem_init(&actor_addr->sem_student, 1, 0);
    if (retval != 0)
    {
        perror(" Couldn ’t initialize. ");
        exit(3);
    }
    //cooks dinamik olduğu için shared memory içinde yer al..
    shm_cook = shmget(IPC_PRIVATE, sizeof(int *), IPC_CREAT | 0644);
    actor_addr->cooks = (int *)shmat(shm_cook, NULL, 0);
    //students dinamik olduğu için shared memory içinde yer al..
    shm_student = shmget(IPC_PRIVATE, sizeof(int *), IPC_CREAT | 0644);
    actor_addr->students = (int *)shmat(shm_student, NULL, 0);

    actor_addr->supplier_finish = 0;
    actor_addr->flagSup = 0;
    /************************* LOCATIONS **d**************************************/

    int shm_kitchen;
    /* Make locations shared memory segment . */
    shm_kitchen = shmget(IPC_PRIVATE, sizeof(Kitchen), IPC_CREAT | 0644);
    if (shm_kitchen == -1)
        perror(" Creation Locations");
    /* Attach kitchen the segment . */
    kitchen_addr = (Kitchen *)shmat(shm_kitchen, NULL, 0);

    retval = sem_init(&kitchen_addr->mutex_kitchen, 1, 1);
    if (retval != 0)
    {
        perror(" Couldn ’t initialize. ");
        exit(3);
    }
    kitchen_addr->desert = 0;
    kitchen_addr->main_course = 0;
    kitchen_addr->soup = 0;
    kitchen_addr->capacity = kitchenSize;

    ////////////////////////////////////////////
    int shm_counter;
    /* Make locations shared memory segment . */
    shm_counter = shmget(IPC_PRIVATE, sizeof(Counter), IPC_CREAT | 0644);
    if (shm_counter == -1)
        perror(" Creation Locations");
    /* Attach counter the segment . */
    counter_addr = (Counter *)shmat(shm_counter, NULL, 0);

    retval = sem_init(&counter_addr->mutex_counter, 1, 1);
    if (retval != 0)
    {
        perror(" Couldn ’t initialize. ");
        exit(3);
    }
    counter_addr->desert = 0;
    counter_addr->main_course = 0;
    counter_addr->soup = 0;
    counter_addr->capacity = counterSize;
    counter_addr->studentNum = studentSize;
    counter_addr->tableNum = tableSize;
    /******************** FLAGS **************************/
    int sdesert = 0, smain_course = 0, ssoup = 0; //flag for student
    int cdesert = 0, cmain_course = 0, csoup = 0; //flag for cook

    /*********************SEM********************************/
    retval = sem_init(&actor_addr->sem_supplier, 1, kitchenSize);
    if (retval != 0)  {
        perror(" Couldn ’t initialize. ");
        exit(3);
    }
    retval = sem_init(&counter_addr->sem_table, 1, tableSize); /////////////// ???? TABLE
    if (retval != 0)  {
        perror(" Couldn ’t initialize. ");
        exit(3);
    }
    retval = sem_init(&counter_addr->sem_cook, 1, counterSize);  // Tezgah boyutu kadar yemek koy..
    if (retval != 0)  {
        perror(" Couldn ’t initialize. ");
        exit(3);
    }

    /////Düzgün dağılmalı ////
    if (counterSize % 3 == 0)
    {
        sem_init(&counter_addr->sem_soup, 1, counterSize / 3);
        sem_init(&counter_addr->sem_main_course, 1, counterSize / 3);
        sem_init(&counter_addr->sem_desert, 1, counterSize / 3);
    }
    else if (counterSize % 3 == 1)
    {
        sem_init(&counter_addr->sem_soup, 1, counterSize / 3);
        sem_init(&counter_addr->sem_main_course, 1, counterSize / 3);
        sem_init(&counter_addr->sem_desert, 1, counterSize / 3 + 1);
    }
    else if (counterSize % 3 == 2)
    {
        sem_init(&counter_addr->sem_soup, 1, counterSize / 3);
        sem_init(&counter_addr->sem_main_course, 1, counterSize / 3 + 1);
        sem_init(&counter_addr->sem_desert, 1, counterSize / 3 + 1);
    }
    int parentPID = getpid();
    int pid = 1;
    int pids[studentSize + cookSize + 1];
    int i;
    for (i = 0; i < studentSize + cookSize + 1; i++) // #student + #cook + #supplier
    {
        if (pid > 0)
        { //paretn içinde forkla ki aynı parent olsun
            pid = fork();
            if (i < cookSize)
            {
                if (pid > 0)
                {
                    actor_addr->cooks[i] = pid;
                    pids[i] = pid;
                }
            }
            else if (i >= cookSize && i < studentSize + cookSize)
            {
                if (pid > 0)
                {
                    actor_addr->students[i - cookSize] = pid;
                    pids[i] = pid;
                }
            }
            else
            {
                if (pid > 0)
                {
                    actor_addr->supplier = pid;
                    pids[i] = pid;
                }
            }
        }
    }

    if (pid > 0)
    {
        ;
    }
    else
    {
        if (whichActor(actor_addr->students, getpid(), studentSize))
        {

            while (rounds > 0)
            {
                
                sem_wait(&actor_addr->sem_student);

                //ihtiyacı olanlardan birer tane alsın..
                if (counter_addr->desert > 0 && counter_addr->main_course > 0 && counter_addr->soup > 0)
                {
                    int csize = counter_addr->soup + counter_addr->main_course + counter_addr->desert;
                    sem_wait(&counter_addr->mutex_counter);
                    fprintf(stderr,"Student%d is going to the counter (round %d) - # of students at counter : %d and counter items P:%d, C:%d, D:%d = %d\n",
                        (getpid() - parentPID -1 - cookSize), rounds, counter_addr->studentNum ,counter_addr->soup,counter_addr->main_course,counter_addr->desert,csize); 
                    counter_addr->desert--;
                    counter_addr->main_course--;
                    counter_addr->soup--;
                    ssoup = 1;
                    smain_course = 1;
                    sdesert = 1;

                    sem_post(&counter_addr->sem_soup);

                    sem_post(&counter_addr->sem_main_course);

                    sem_post(&counter_addr->sem_desert);

                    sem_post(&counter_addr->mutex_counter);
                }

                if (sdesert && smain_course && ssoup)
                {
                    sem_wait(&counter_addr->sem_table);
                    fprintf(stderr,"Student%d got food and is going to get a table (round %d) - #of empty tables: %d\n",
                        (getpid()-parentPID-cookSize-1), rounds, counter_addr->tableNum); 

                    counter_addr->tableNum--;
                    ssoup = 0;
                    smain_course = 0;
                    sdesert = 0;
                    rounds--;
                    
                    int num = rand() % tableSize;
                    fprintf(stderr,"Student%d sat at table %d to eat (round %d) - #of empty tables: %d\n",
                        (getpid()-parentPID-cookSize-1), num, rounds, counter_addr->tableNum);

                    if (actor_addr->flagSup == 1 || kitchen_addr->desert + kitchen_addr->main_course + kitchen_addr->soup >= kitchenSize)
                    {
                        int i;
                        for (i = 0; i < cookSize; i++)
                        {
                            sem_post(&actor_addr->sem_cook);
                        }
                    }
                    int i;
                    for (i = 0; i < 3; i++)
                    {
                        sem_post(&counter_addr->sem_cook); // tezgahta 3 yer boşaldı diye haber yollar..
                    }
                    counter_addr->tableNum++;
                    num = rand() % tableSize;
                    fprintf(stderr,"Student%d left table %d to eat again (round %d) - #of empty tables: %d\n",
                        (getpid()-parentPID-cookSize-1), num, rounds +1, counter_addr->tableNum); 
                    sem_post(&counter_addr->sem_table);
                }
            }
            fprintf(stderr,"Student%d is done eating L=%d times -  going home - GOODBYE!!!\n",(getpid()-parentPID-1-cookSize),rounds);
            counter_addr->studentNum--;
        }

        //COOKS PART
        if (whichActor(actor_addr->cooks, getpid(), cookSize))
        {   
            int kitchenFull = 0;
            while (!(actor_addr->supplier_finish == 1))
            {
                if (rounds == 0 && counter_addr->desert + counter_addr->soup + counter_addr->main_course == 0 && kitchen_addr->soup + kitchen_addr->main_course + kitchen_addr->desert == 0)
                {  exit(0);  }

                //decrease cook size that working on kitchen
                sem_wait(&actor_addr->sem_cook);
                int control = 0;
                sem_wait(&kitchen_addr->mutex_kitchen);
                int ksize = kitchen_addr->soup + kitchen_addr->main_course + kitchen_addr->desert;
                
                fprintf(stderr,"Cook %d is going to the kitchen to wait for/get a plate - kitchen items: P:%d,C:%d,D:%d = %d\n",
                        (getpid() - parentPID -1) ,kitchen_addr->soup, kitchen_addr->main_course, kitchen_addr->desert, ksize); 

                int dsem, msem, ssem;
                sem_getvalue(&counter_addr->sem_desert, &dsem);
                sem_getvalue(&counter_addr->sem_soup, &ssem);
                sem_getvalue(&counter_addr->sem_main_course, &msem);
                
                int csize = counter_addr->soup + counter_addr->main_course + counter_addr->desert;

                if (dsem != 0 && kitchen_addr->desert > 0)
                {   //tatlı tezgahta eksik..
                    cdesert++;
                    kitchen_addr->desert--;
                    fprintf(stderr, "Cook%d is going to the counter to deliver desert - counter items : P:%d, C:%d, D:%d = %d\n",
                            (getpid()-parentPID-1), counter_addr->soup, counter_addr->main_course, counter_addr->desert,csize);
                    if (actor_addr->flagSup == 0)
                        sem_post(&actor_addr->sem_supplier);
                    if (actor_addr->flagSup == 1)
                    {
                        sem_post(&actor_addr->sem_cook);
                    }
                    control = 1;
                }
                else if (ssem != 0 && kitchen_addr->soup > 0)
                { //çorba tezgahta eksik..
                    csoup++;
                    kitchen_addr->soup--;
                    fprintf(stderr, "Cook%d is going to the counter to deliver soup - counter items : P:%d, C:%d, D:%d = %d\n", 
                                (getpid()-parentPID-1), counter_addr->soup, counter_addr->main_course, counter_addr->desert,csize);
                    if (actor_addr->flagSup == 0)
                        sem_post(&actor_addr->sem_supplier);
                    if (actor_addr->flagSup == 1)
                    {
                        sem_post(&actor_addr->sem_cook);
                    }
                    control = 1;
                }
                else if (msem != 0 && kitchen_addr->main_course > 0)
                { //anayemek tezgahta eksik..

                    cmain_course++;
                    kitchen_addr->main_course--;
                    fprintf(stderr, "Cook%d is going to the counter to deliver main course - counter items : P:%d, C:%d, D:%d = %d\n", 
                            (getpid()-parentPID-1), counter_addr->soup, counter_addr->main_course, counter_addr->desert,csize);
                    if (actor_addr->flagSup == 0)
                        sem_post(&actor_addr->sem_supplier);
                    if (actor_addr->flagSup == 1)
                    {
                        sem_post(&actor_addr->sem_cook);
                    }
                    control = 1;
                }
                sem_post(&kitchen_addr->mutex_kitchen);

                //tezgahı kitle, dağıtım yapsın
                while (control == 1)
                {
                    //tezgaha tabak koyar
                    if (cdesert > 0)
                    {
                        sem_wait(&counter_addr->sem_desert);
                        sem_wait(&counter_addr->mutex_counter);

                        cdesert--;
                        counter_addr->desert++;
                        fprintf(stderr, "Cook%d is placed desert on the counter - counter items : P:%d, C:%d, D:%d = %d\n", 
                            (getpid()-parentPID-1), counter_addr->soup, counter_addr->main_course, counter_addr->desert,csize+1);
                        control = 0;

                        sem_post(&counter_addr->mutex_counter);
                    }
                    else if (cmain_course > 0)
                    {
                        sem_wait(&counter_addr->sem_main_course); //
                        sem_wait(&counter_addr->mutex_counter);

                        cmain_course--;
                        counter_addr->main_course++;
                        fprintf(stderr, "Cook%d is placed main course on the counter - counter items : P:%d, C:%d, D:%d = %d\n", 
                            (getpid()-parentPID-1), counter_addr->soup, counter_addr->main_course, counter_addr->desert,csize+1);
                        control = 0;

                        sem_post(&counter_addr->mutex_counter);
                    }
                    else if (csoup > 0)
                    {
                        sem_wait(&counter_addr->sem_soup);
                        sem_wait(&counter_addr->mutex_counter);
                        
                        csoup--;
                        counter_addr->soup++;
                        
                        fprintf(stderr, "Cook%d is placed soup on the counter - counter items : P:%d, C:%d, D:%d = %d\n", 
                            (getpid()-parentPID-1), counter_addr->soup, counter_addr->main_course, counter_addr->desert,csize+1); 
                        
                        control = 0;
                        
                        sem_post(&counter_addr->mutex_counter);
                    }
                    //tüm öğrencileri uyandır.. yemek alsınlar
                    if (counter_addr->desert > 0 && counter_addr->main_course > 0 && counter_addr->soup > 0)
                    {
                        int x = studentSize;
                        while (x > 0)
                        {
                            sem_post(&actor_addr->sem_student);
                            x--;
                        }
                    }
                }
            }
            fprintf(stderr,"Cook%d finished serving - items at kitchen : 0--- going home - GOODBYE!!!\n",(getpid()-parentPID-1));
        }
        // SUPPLIER PART
        if (actor_addr->supplier == getpid())
        {

            const int size = 3 * rounds * studentSize; // öğrenci * yemek yeme sayısı * 3(yemek çeşidi)
            char *meals = (char *)malloc(size * sizeof(char));
            char c;
            int i;
            for (i = 0; i < size; i++)
            {
                read(fd, &c, 1);
                if(c != 'P' && c != 'D' && c != 'C') {
                    perror("Not valid food!");
                    break;
                }
                meals[i] = c;
            }
            close(fd);
            if( i != size ) {
                perror("Not enough input in file !");
                free(meals);
                int j;
                exitProgram(actor_addr,kitchen_addr,counter_addr);
                for ( j = 0; j < studentSize+cookSize+1; j++)
                {
                    kill(pids[j],SIGTERM);
                }
                
                return -1;
            }

            while (readIndex < size)
            {
                //decrease supplier 
                sem_wait(&actor_addr->sem_supplier);
                //cooks should not enter kitchen when supplier ins
                sem_wait(&kitchen_addr->mutex_kitchen); //Entering kitchen
                //give a plate
                int ksize = kitchen_addr->soup + kitchen_addr->main_course + kitchen_addr->desert;
                switch (meals[readIndex++])
                {
                case 'P':
                    fprintf(stderr,"The supplier is going to the kitchen to deliver soup: kitchen items: P: %d, C:%d, D:%d = %d\n", 
                        kitchen_addr->soup, kitchen_addr->main_course, kitchen_addr->desert, ksize); 
                    kitchen_addr->soup++;
                    fprintf(stderr,"The supplier delivered soup: kitchen items: P: %d, C:%d, D:%d = %d\n", 
                        kitchen_addr->soup, kitchen_addr->main_course, kitchen_addr->desert, ksize); 
                    break;
                case 'C':
                    fprintf(stderr,"The supplier is going to the kitchen to deliver main course: kitchen items: P: %d, C:%d, D:%d = %d\n", 
                        kitchen_addr->soup, kitchen_addr->main_course, kitchen_addr->desert, ksize); 
                    kitchen_addr->main_course++;
                    fprintf(stderr,"The supplier delivered main course: kitchen items: P: %d, C:%d, D:%d = %d\n", 
                        kitchen_addr->soup, kitchen_addr->main_course, kitchen_addr->desert, ksize); 
                    break;
                case 'D':
                    fprintf(stderr,"The supplier is going to the kitchen to deliver desert: kitchen items: P: %d, C:%d, D:%d = %d\n", 
                        kitchen_addr->soup, kitchen_addr->main_course, kitchen_addr->desert, ksize);
                    kitchen_addr->desert++;
                    fprintf(stderr,"The supplier delivered desert: kitchen items: P: %d, C:%d, D:%d = %d\n", 
                        kitchen_addr->soup, kitchen_addr->main_course, kitchen_addr->desert, ksize); 
                    break;
                default:
                    perror("Not valid food");
                    break;
                }
                sem_post(&actor_addr->sem_cook);
                sem_post(&kitchen_addr->mutex_kitchen);
            }
            fprintf(stderr,"The supplier finished spplying - GOODBYE!\n");
            actor_addr->flagSup = 1;
            int y = kitchen_addr->desert + kitchen_addr->soup + kitchen_addr->main_course;

            int kontrol3 = 1;
            while (kontrol3 == 1)
            {
                if (kitchen_addr->desert + kitchen_addr->main_course + kitchen_addr->soup > 0 ||
                    counter_addr->desert + counter_addr->main_course + counter_addr->soup > 0)
                {
                    sem_post(&actor_addr->sem_cook);
                }
                else
                {
                    actor_addr->supplier_finish = 1;
                    kontrol3 = 0;
                }
            }
        }
    }

    /* wait for child process's terination. */

    pid_t child_pid, wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0)
    {
        int value;
        sem_getvalue(&actor_addr->sem_cook, &value);
        if (value == 0 && actor_addr->flagSup == 1)
        {
            //mutfakta yemek var mı ?
            if (kitchen_addr->soup + kitchen_addr->desert + kitchen_addr->main_course > 0 &&
                (counter_addr->soup < counterSize / 3 || counter_addr->desert < counterSize / 3 || counter_addr->main_course < counterSize / 3))
            {
                sem_post(&actor_addr->sem_cook);
                sem_post(&counter_addr->sem_cook);
            }
            else if (kitchen_addr->soup + kitchen_addr->desert + kitchen_addr->main_course == 0 &&
                     (counter_addr->soup + counter_addr->desert + counter_addr->main_course == 0))
            {
                int i;
                for (i = 0; i < studentSize + cookSize + 1; i++)
                {
                    kill(pids[i], SIGTERM);
                }
            }
        };
    }

    if (actor_addr->supplier_finish == 1)
    {

        exitProgram(actor_addr,kitchen_addr,counter_addr);
        signal(SIGQUIT, SIG_IGN);
        kill(getpid(), SIGQUIT); 
    } 
    return 0;
}
int whichActor(int *arr, int pid, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (arr[i] == pid)
            return 1;
    }
    return 0;
}

void exitProgram(Actors* actor_addr, Kitchen* kitchen_addr, Counter* counter_addr) {
    
    //free Kitchen
    sem_destroy(&kitchen_addr->mutex_kitchen);

    //free Counter
    sem_destroy(&counter_addr->mutex_counter);
    sem_destroy(&counter_addr->sem_desert);
    sem_destroy(&counter_addr->sem_main_course);
    sem_destroy(&counter_addr->sem_soup);
    sem_destroy(&counter_addr->sem_cook);
    //free Table
    sem_destroy(&counter_addr->sem_table);

    //free Actor - Student
    sem_destroy(&actor_addr->sem_student);
    //free Actor - Cook
    sem_destroy(&actor_addr->sem_cook);
    //free Actor - Supplier
    sem_destroy(&actor_addr->sem_supplier);


    signal(SIGQUIT, SIG_IGN);
    kill(getpid(), SIGQUIT);

       
    if(shmdt(actor_addr->students) == -1 )
        perror("shmdt actor student");
    if(shmdt(actor_addr->cooks) == -1 )
        perror("shmdt actor student");
    if(shmdt(actor_addr) == -1 )
        perror("shmdt actor");
    if(shmdt(counter_addr) == -1 )
        perror("shmdt counter");
    if(shmdt(kitchen_addr) == -1 )
        perror("shmdt kitchen");
}