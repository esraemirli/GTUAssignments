#include<stdio.h>
int recursion(char *str,int size,int i,int level);

void main(void){

    char str[100];
    int size=0;

	//Reads file and stores to string called is str.
    FILE *fp;
    fp=fopen("input.txt","r");
    while(!feof(fp) && fp!='\0'){
        fscanf(fp,"%c",&str[size]);
        size++;
    }
    size-=2;
    recursion(str,size,0,0);

}
int recursion(char *str,int size,int i,int level){

    int j;  
	//When values equal to '(', level increases.
    if(str[i]=='('){
        i++;
        level++;
    }   
    if(str[i]!=')' && str[i]!='(') {
        for(j=0;j<level;j++)
            printf("-");
        printf("%c\n",str[i]);
        i++;
    }
    //When values equal to ')', level decreases.
    if(str[i]==')'){
        level--;
        i++;
    }
    if(str[i]==',')
        i++; 
    //Send last value of index and level.
    if(i<size)
        recursion(str,size,i,level); 
}

