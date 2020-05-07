#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <complex.h>

double PI;
typedef double complex cplx;
int timer;
int isEmptyLine(char *, int);
int giveLine(char *, int);
int printEmptyLine(char *, int);
int numberOfLines(char *);
int printResult(char *, cplx buf[]);
void calculateFile3(int, int, char *);
char commA(int timer, int id);

//  0         1     2       3      4       5  6
//./programA -i inputPathA -o outputPathA -t time
int main(int argc, char *argv[])
{
   if (argc != 7)
   {
      printf("USAGE : ./programB -i inputPathB -o outputPathB -t time\n");
      return -1;
   }
   const int LINE = 160;
   int opt;
   char *inputPath, *outputPath;
   while ((opt = getopt(argc, argv, "i:t:o:")) != -1)
   {
      switch (opt)
      {
      case 'o':
         outputPath = optarg;
         break;
      case 't':
         timer = atoi(optarg);
         break;
      case 'i':
         inputPath = optarg;
         break;
      case ':':
         printf("Option needs a value %s\n", optarg);
         printf("USAGE : ./programB -i inputPathB -o outputPathB -t time\n");
         return -1;
      case '?':
         printf("Unknown option: %c\n", optopt);
         printf("USAGE : ./programB -i inputPathB -o outputPathB -t time\n");
         return -1;
      }
   }
   for (; optind < argc; optind++)
   {
      printf("Extra arguments: %s\n", argv[optind]);
      printf("USAGE : ./programB -i inputPathB -o outputPathB -t time\n");
      return -1;
   }
   if (strcmp(outputPath, inputPath) == 0)
   {
      printf("Outputpath and inputPath cannot be the same. \n");
      return -1;
   }
   if (timer > 50 || timer < 1)
   {
      printf("Time range is [1,50] -t [1,50]\n");
      return -1;
   }
   /////////////////////////////////////////////////////////////////
   int fdRead;
   struct flock readLock;
   memset(&readLock, 0, sizeof(readLock));

   fdRead = open(inputPath, O_RDWR | O_CREAT);
   if (fdRead == -1)
   {
      perror("Inputpath Error: ");
      return -1;
   }
   unsigned char buf[32];

   ssize_t bytes_read;
   // waits input from a
   commA(timer, 1);
   while (1)
   {
      readLock.l_type = F_WRLCK;
      while (fcntl(fdRead, F_SETLK, &readLock) == -1)
      {
         usleep(timer);
      }

      srand(time(0));
      int randomNumber = rand() % numberOfLines(inputPath);
      int firstRandom = randomNumber;
      int skipByte = giveLine(inputPath, randomNumber); // , rand() % 10 -> 10 yerine satır sayısı gelicek !!
                                                        // random verilen satir basina gidiyor
      int isEmpty = isEmptyLine(inputPath, skipByte);   // satir bos mu bakiyor

      while (isEmpty == 1 || isEmpty == 0) // 1 satir bos , 0 dosya sonu
      {
         if (isEmpty == 0)
            randomNumber = -1;
         randomNumber++;

         skipByte = giveLine(inputPath, randomNumber); // , rand() % 10 -> 10 yerine satır sayısı gelicek !!
         isEmpty = isEmptyLine(inputPath, skipByte);

         if (firstRandom == randomNumber)
         {

            if (commA(timer, -1) == 'a')
            {
               remove("esra.txt");
            }
            close(fdRead);
            return 0;
         }
      }
      //skipByte ile satıra git oku complex çöz döndür...
      calculateFile3(fdRead, skipByte, outputPath);
      printEmptyLine(inputPath, skipByte);

      readLock.l_type = F_UNLCK;
      fcntl(fdRead, F_SETLK, &readLock);
      usleep(timer);
   }

   close(fdRead);

   return 0;
}

int numberOfLines(char *fileName)
{
   char buf[1];
   int satir = 0;
   int fd = open(fileName, O_RDONLY);
   if (fd == -1)
   {
      perror("Error :");
      return -1;
   }
   while (read(fd, buf, sizeof(buf)) > 0)
   {
      if (buf[0] == '\n')
      {
         satir++;
      }
   }
   close(fd);
   if(satir==0)
      satir = 1;
   return satir;
}

// 1 for blankLine , -1 for not blank , 0 for end of file
int isEmptyLine(char *path, int offset)
{
   char buf[1];
   int fd = open(path, O_RDWR);
   if (fd == -1)
   {
      perror("Error :");
      return -1;
   }
   lseek(fd, offset, SEEK_SET);
   while (read(fd, buf, sizeof(buf)) > 0)
   {
      if (buf[0] != '\n' && buf[0] != ' ')
      {
         close(fd);
         return -1;
      }
      else if (buf[0] == '\n')
      {
         close(fd);
         return 1;
      }
   }
   close(fd);
   return 0;
}

int giveLine(char *path, int randomLine)
{
   char buf[1];
   int newline = 0;
   int start = 0, finish = -1;

   int random = randomLine;
   int bytes_read = 0, other = 0;
   int fd = open(path, O_RDWR);
   if (fd == -1)
   {
      perror("Error :");
      return -1;
   }
   while (read(fd, buf, sizeof(buf)) > 0)
   {
      bytes_read++; //7 - 14
      if (buf[0] == '\n')
      {
         newline++;
         if (newline == random - 1)
         {
            start = bytes_read;
         }
         if (newline == random)
            finish = bytes_read;
      }
   }
   if (finish == -1)
      finish = bytes_read;
   newline++;

   close(fd);
   return start;
}

int printEmptyLine(char *path, int startWrite)
{
   struct flock lock;
   memset(&lock, 0, sizeof(lock));
   int file = open(path, O_WRONLY);
   if (file == -1)
   {
      perror("Error :");
      return -1;
   }
   lock.l_type = F_WRLCK;
   while (fcntl(file, F_SETLK, &lock) == -1)
   {
      usleep(timer);
   }
   lseek(file, startWrite, SEEK_SET);
   for (int i = 0; i < 160; i++)
   {
      write(file, " ", 1);
   }
   write(file, "\n", 1);
   // usleep(100);
   lock.l_type = F_UNLCK;
   fcntl(file, F_SETLK, &lock);

   close(file);
   return 0;
}

int printResult(char *outputFile, cplx buf[])
{
   struct flock lock;
   memset(&lock, 0, sizeof(lock));

   int file = open(outputFile, O_APPEND | O_RDWR);
   if (file == -1)
   {
      perror("Error :");
      return -1;
   }
   lock.l_type = F_WRLCK;
   while (fcntl(file, F_SETLK, &lock) == -1)
   {
      usleep(timer);
   }

   //   char temp[50];

   for (int i = 0; i < 16; i++)
   {
      double real = creal(buf[i]);
      double img = cimag(buf[i]);
      double *rnum, *inum;
      int ret = -1;
      rnum = (double *)&real;
      inum = (double *)&img;
      char rbuf[8] = {0}; //to use with write() for real
      char ibuf[8] = {0}; //to use with write() for image
      memset(rbuf, 0, sizeof(rbuf));
      snprintf(rbuf, sizeof *rnum, "%0.3lf", *rnum); //print to buffer real

      memset(ibuf, 0, sizeof(ibuf));
      snprintf(ibuf, sizeof *inum, "%0.3lf", *inum); //print to buffer img

      write(file, "(", 1);
      write(file, rbuf, strlen(rbuf)); //write the buf to file
      write(file, ",", 1);
      write(file, ibuf, strlen(ibuf)); //write the buf to file
      write(file, "), ", 3);
   }

   write(file, "\n", 1);
   //usleep(100);
   lock.l_type = F_UNLCK;
   fcntl(file, F_SETLK, &lock);

   close(file);
   return 0;
}
//https://rosettacode.org/wiki/Fast_Fourier_transform#C
void _fft(cplx buf[], cplx out[], int n, int step)
{
   if (step < n)
   {
      _fft(out, buf, n, step * 2);
      _fft(out + step, buf + step, n, step * 2);

      for (int i = 0; i < n; i += 2 * step)
      {
         cplx t = cexp(-I * PI * i / n) * out[i + step];
         buf[i / 2] = out[i] + t;
         buf[(i + n) / 2] = out[i] - t;
      }
   }
}

void fft(cplx buf[], int n)
{
   cplx out[n];
   for (int i = 0; i < n; i++)
      out[i] = buf[i];

   _fft(buf, out, n, 1);
}

void calculateFile3(int fd, int startRead, char *fdWrite)
{
   char buf[1];
   int bs = 0;
   int bytes_read = 0;

   lseek(fd, startRead, SEEK_SET);
   cplx cplx[16];
   char temp[3];
   int real, img;
   int i = 0, c = 0;

   while (read(fd, buf, sizeof(buf)) > 0 && buf[0] != ' ' && buf[0] != '\n')
   {
      if (buf[0] >= '0' && buf[0] <= '9')
      {
         temp[i] = buf[0];
         i++;
      }

      else if (buf[0] == ',')
      {
         if (i == 2)
            temp[2] = '\0';
         i = 0;
         img = atoi(temp);

         double complex z = real + img * I;
         cplx[c] = z;
         c++;
      }
      else if (buf[0] == 'i')
      {
         if (i == 2)
            temp[2] = '\0';
         i = 0;
         real = atoi(temp);
      }
   }
   PI = 3.141593;
   fft(cplx, c);

   printResult(fdWrite, cplx);
}

char commA(int timer, int id)
{
   struct flock lockForA;
   char ch;
   int fdx;
   while ((fdx = open("esra.txt", O_RDWR)) == -1)
   {
      printf("There is no output from program A yet\n");
      usleep(timer);
   }

   memset(&lockForA, 0, sizeof(lockForA));

   lockForA.l_type = F_WRLCK;
   while (fcntl(fdx, F_SETLK, &lockForA) == -1)
   {
      usleep(timer);
   }

   read(fdx, &ch, 1);
   ch = ch + id;
   lseek(fdx, 0, SEEK_SET);
   write(fdx, &ch, 1);

   lockForA.l_type = F_UNLCK;
   fcntl(fdx, F_SETLK, &lockForA);
   close(fdx);
   return ch;
}