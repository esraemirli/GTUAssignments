#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <wait.h>

#include "singular_value_decomposition.c"



#define ROW 30
#define COL 30
double U[ROW][COL];                                                       
double V[COL][COL];                                                        
double singular_values[COL];                                             
double* dummy_array; 
int err;


int exponential(int,int);
int inputPathSize(char* );

int childCount = 0;
int wakeUp = 0;
int wpid, status = 0;