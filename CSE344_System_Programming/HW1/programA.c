#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int getValidLine(char *);
int timer;
int main(int argc, char **argv)
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
   int check = open("esra.txt", O_RDONLY);
   if (check == -1)
   {
      struct flock lockForA;
      lockForA.l_type = F_WRLCK;

      int x = open("esra.txt", O_CREAT | O_WRONLY, 0777);
      fcntl(x, F_SETLK, &lockForA);

      write(x, "a", 1);

      lockForA.l_type = F_UNLCK;
      fcntl(x, F_SETLK, &lockForA);

      close(x);
   }
   else
   {
      close(check);
   }
   /////////////////////////
   int fdWrite, fdRead;
   struct flock lock;

   fdRead = open(inputPath, O_RDONLY);
   if (fdRead == -1)
   {
      perror("Inputpath Error: ");
      return -1;
   }
   memset(&lock, 0, sizeof(lock));

   unsigned char buf[32];

   ssize_t bytes_read;

   fdWrite = open(outputPath, O_RDWR);
   if (fdWrite == -1)
   {
      perror("OutputPath Error: ");
      close(fdRead);
      return -1;
   }
   bytes_read = read(fdRead, buf, sizeof(buf));
   while (bytes_read == sizeof(buf))
   {
      lock.l_type = F_WRLCK;
      while (fcntl(fdWrite, F_SETLK, &lock) == -1)
      {
         usleep(timer);
      }

      lseek(fdWrite, getValidLine(outputPath) - 1, SEEK_SET);
      int sumOfChars = 0;
      for (int i = 0; i < bytes_read; i++)
      {
         int ascii = buf[i];
         int *num;
         int ret = -1;
         num = (int *)&ascii;
         char buf[4] = {0}; //to use with write()

         memset(buf, 0, sizeof(buf));
         snprintf(buf, sizeof *num, "%d", *num); //print to buffer

         if (i % 2 != 0)
         {
            sumOfChars += write(fdWrite, "+i", 2);
            sumOfChars += write(fdWrite, buf, strlen(buf)); //write the buf to file
            sumOfChars += write(fdWrite, ",", 1);
         }
         else
            sumOfChars += write(fdWrite, buf, strlen(buf)); //write the buf to file
      }

      for (int i = 0; i < LINE - sumOfChars; i++)
         write(fdWrite, " ", 1);

      write(fdWrite, "\n", 1);
      //      usleep(100);
      lock.l_type = F_UNLCK;
      fcntl(fdWrite, F_SETLK, &lock);

      //      srand(time(0));
      //      usleep(rand() % 5 + 1);

      bytes_read = read(fdRead, buf, sizeof(buf));
   }
   close(fdWrite);
   close(fdRead);
   return 0;
}

// gets first empty line
int getValidLine(char *path)
{
   // BS : backslash
   char buf[1];
   int firstPoint = 0;
   int bytes_read = 0, otherCharCount = 0;
   int fd = open(path, O_RDONLY);

   while (read(fd, buf, sizeof(buf)) > 0)
   {
      bytes_read++;
      if (buf[0] != '\n' && buf[0] != ' ')
      {
         otherCharCount++;
      }
      else if (buf[0] == '\n')
      {
         if (otherCharCount == 0)
         {
            close(fd);
            return firstPoint + 1;
         }
         firstPoint = bytes_read;
         otherCharCount = 0;
      }
   }

   close(fd);
   return bytes_read > 0 ? bytes_read + 1 : 0;
}