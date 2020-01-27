#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct identifier{
    char name[10];
    int value;
};

struct gate{
    char name1[10],name2[10],name3[10];
    int type,input1,input2,output;
};

int input_size(int size);
int input_row(int iRow);
int circuit_row(int cRow);

void main(){

    struct identifier *inputlist;
    struct gate *gatelist;

    FILE *file;
    FILE *input;

    int sizeid=0,iRow=0,cRow=0;
    char ch[10];
    int num;     
    int i,j,k,l;


    //Checks how many inputs are in circuit txt
    sizeid=input_size(sizeid);
    //Checks how many lines are in input txt 
    iRow=input_row(iRow); 
    //Checks how many lines are in circuit txt
    cRow=circuit_row(cRow);

    //Makes room in memory for identifier and gate structs
    inputlist = malloc(sizeid * sizeof(struct identifier));
    gatelist = malloc((cRow-1) * sizeof(struct gate));


    //Saves input names in inputlist
    file = fopen("circuit.txt","r");    
    while(fscanf(file,"%s",ch)){
        for(i = 0; i<sizeid; ++i){
            fscanf(file,"%s",ch);
            strcpy(inputlist[i].name,ch);
        }
        break;
    }
    fclose(file);


    file = fopen("circuit.txt","r");   
    i=0;
    k=0;
    //Circiut informations copied to gate struct line by line. **name3 is output name2 and name1 input**
    while(i<cRow){        
        fscanf(file,"%s",ch);
        //If ch is input read as empty
        if(strcmp(ch,"INPUT") == 0){
            for(i = 0; i<sizeid; ++i){
                fscanf(file,"%s",ch);
            }
            i = 0;
        }
        else if(strcmp(ch,"AND") == 0){
            gatelist[k].type = 0;
            
            //First readed output is name3
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name3,ch);
    
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name2,ch);
        
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name1,ch);
            k++;
        }
        else if(strcmp(ch,"OR") == 0){
            gatelist[k].type = 1;
            
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name3,ch);
    
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name2,ch);
        
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name1,ch);
            k++;
        }
        else if(strcmp(ch,"NOT") == 0){
            gatelist[k].type = 2;
            
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name3,ch);
        
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name1,ch);
            k++;
        }
        else if(strcmp(ch,"FLIPFLOP") == 0){
            gatelist[k].type = 3;

            //First value of flipflop is 0
            gatelist[k].output = 0;

            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name3,ch);
        
            fscanf(file,"%s",ch);
            strcpy(gatelist[k].name1,ch);
            k++;
        }
        ++i;
    }
    fclose(file);


    input = fopen("input.txt","r");
    for(i = 0; i<iRow; ++i){
        //Input values transferred to inputlist line by line
        while(j<sizeid){
            fscanf(input,"%d",&num);
            inputlist[j].value=num;
            ++j;
        }
        j = 0;


        for(k=0; k<(cRow-1); ++k){        
            //Doing all the calculations for AND
            if(gatelist[k].type == 0){
                //Checks if the input in inputlist if true get values 
                for(l=0; l<sizeid; l++){
                    if(strcmp(gatelist[k].name2,inputlist[l].name) == 0){
                        gatelist[k].input2 = inputlist[l].value;
                    }
                    if(strcmp(gatelist[k].name1,inputlist[l].name) == 0){        
                        gatelist[k].input1 = inputlist[l].value;
                    }
                }
                l = 0;
                //Checks if the input another gates output
                while(l<k){
                    if(strcmp(gatelist[k].name2,gatelist[l].name3) == 0){
                        gatelist[k].input2 = gatelist[l].output;        
                    }
                    if(strcmp(gatelist[k].name1,gatelist[l].name3) == 0){
                        gatelist[k].input1 = gatelist[l].output;
                    }
                    ++l;
                }
                //Calculates output
                if(gatelist[k].input1 == 0 || gatelist[k].input2 == 0){
                    gatelist[k].output = 0;
                }
                else{ 
                    gatelist[k].output = 1;
                }
            
                if(k == cRow-2){
                    printf("%d\n",gatelist[k].output);
                }
            }

            //Doing all the calculations for OR
            else if(gatelist[k].type == 1){
                //Checks if the input in inputlist if true get values 
                for(l=0; l<sizeid; l++){
                    if(strcmp(gatelist[k].name2,inputlist[l].name) == 0){
                        gatelist[k].input2 = inputlist[l].value;
                    }
                    if(strcmp(gatelist[k].name1,inputlist[l].name) == 0){        
                        gatelist[k].input1 = inputlist[l].value;
                    }
                }

                //Checks if the input another gates output
                l = 0;
                while(l<k){
                    if(strcmp(gatelist[k].name2,gatelist[l].name3) == 0){
                        gatelist[k].input2 = gatelist[l].output;
                    }
                    if(strcmp(gatelist[k].name1,gatelist[l].name3) == 0){
                        gatelist[k].input1 = gatelist[l].output;
                    }
                    ++l;
                }
                //Calculates output
                if(gatelist[k].input1 == 1 || gatelist[k].input2 == 1){
                    gatelist[k].output = 1;
                }
                else{ 
                    gatelist[k].output = 0;
                }
                if(k == cRow-2){
                    printf("%d\n",gatelist[k].output);
                }
            }
            //Doing all the calculations for NOT
            else if(gatelist[k].type == 2){
                for(l=0; l<sizeid; l++){
                    if(strcmp(gatelist[k].name1,inputlist[l].name) == 0){
                        gatelist[k].input1 = inputlist[l].value;
                    }
                }
                l=0;
                while(l<k){
                    if(strcmp(gatelist[k].name1,gatelist[l].name3) == 0){
                        gatelist[k].input1 = gatelist[l].output;
                    }
                    ++l;
                }
                if(gatelist[k].input1 == 0 ){
                    gatelist[k].output = 1;
                }
                else{ 
                    gatelist[k].output = 0;
                }
                if(k == cRow-2){
                    printf("%d\n",gatelist[k].output);
                }
            }
            //Doing all the calculations for FLIPFLOP
            else if(gatelist[k].type == 3){
                for(l=0; l<sizeid; l++){
                    if(strcmp(gatelist[k].name1,inputlist[l].name) == 0){
                        gatelist[k].input1 = inputlist[l].value;
                    }
                }
                l=0;
                while(l<k){
                    if(strcmp(gatelist[k].name1,gatelist[l].name3) == 0){
                        gatelist[k].input1 = gatelist[l].output;
                    }
                    ++l;
                }
                if(gatelist[k].input1 == gatelist[k].output){
                    gatelist[k].output = 0;
                }
                else{ 
                    gatelist[k].output = 1;
                }
                if(k == cRow-2){
                    printf("%d\n",gatelist[k].output);
                }
            }
        }
    }
    fclose(input);

    //Empties the used areas in memory
    free(inputlist);
    free(gatelist);
}

//This function reads only the first line of circuit txt and identify identifiers.
int input_size(int size){ //kaç tane input

    FILE *file;
    file = fopen("circuit.txt", "r");

    char ch[10],s;
    size = 0;

    fscanf(file, "%s%c",ch,&s);
    while (s!='\n' && file!=NULL) {
        fscanf(file, "%s%c",ch,&s);
        size++;
    }
    fclose(file);

    return size;
}
//Finds how many lines in input txt
int input_row(int iRow){   
    char value,s;

    FILE *input;
    input=fopen("input.txt","r");
    iRow=0;
    while(!feof(input) && input!=NULL){
        fscanf(input,"%c%c",&value,&s);
        if(s=='\n')
            iRow++;
    }
    fclose(input);
    iRow-=1;

    return iRow;
} 

//Finds how many lines of gates in circuit txt
int circuit_row(int cRow){
    char ch[10],s;

    FILE *file;
    file=fopen("circuit.txt","r");

    cRow=0;
    while(!feof(file) && file!=NULL){
        fscanf(file,"%s%c",ch,&s);
        if(s=='\n')
            cRow++;
    }
    cRow-=1;

    return cRow;
}
