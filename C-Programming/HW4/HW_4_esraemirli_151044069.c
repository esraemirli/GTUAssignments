#include<stdio.h>
#include <ctype.h>
#define MAX 100

void main(void){
    char polynomial[MAX];
    int p_size=0;

    double values[MAX];
    int v_size=0;

    double str[MAX];
    int s_size;
    double result=0;

    char temp[MAX];
    double temp_value,sign;
    int t_size;

    int i,j,k,num=0;

    FILE *fp;
    //read values text and store double array that called values.
    fp=fopen("values.txt","r");
    while(!feof(fp)){
        fscanf(fp,"%lf",&values[v_size]);
        v_size++;
    }
    fclose(fp); 
    v_size-=1;

    //read polynomial text and store char array that called polynomial.
    fp=fopen("polynomial.txt","r");
    while(!feof(fp)){
        fscanf(fp,"%c",&polynomial[p_size]);
        p_size++;
    }
    fclose(fp);
    p_size-=1;
    
    //Delete last space.
    for(i=p_size-1;!isdigit(polynomial[i]); i--)
        p_size--;


    //Opened evaluations text and write results by one by.
    fp= fopen("evaluations.txt","w++");
    while(num<v_size) {

        s_size=0;
        for(i=0;i<p_size;i++,s_size++){
            //When i is a space, increase i.
            while(polynomial[i]==' ' && polynomial[i+1]!='\0'){
               i++;
            }
            
            if( isdigit(polynomial[i] ) ){
                //Check i until is not a digit and store temp. 
                t_size=0;      
                while( isdigit( polynomial[i] ) || polynomial[i]=='.' ){
                        temp[t_size]=polynomial[i];
                        t_size++;
                        i++;
                }
                i--;
                //Change temp string as double value. 
                sscanf(temp,"%lf",&temp_value);
                str[s_size]=temp_value;

                //Reset temp and temp value.
                t_size-=1;
                while(t_size>=0){
                    temp[t_size]='\0';
                    t_size--;
                }
                temp_value='\0';
            }

            else if(polynomial[i]=='x' ){
                str[s_size]=values[num];

                if(polynomial[i+1]=='^'){
                    //Power value can be two-digit.Check power value until is not a digit and store temp. 
                    t_size=0;      
                    while( isdigit( polynomial[i+2] ) || polynomial[i+2]=='.' ){
                            temp[t_size]=polynomial[i+2];
                            t_size++;
                            i++;
                    }
                    i--;

                    //Change temp string as double value. 
                    sscanf(temp,"%lf",&temp_value);

                    //pow function
                    for(j=1;j<temp_value;j++){
                        str[s_size]*=values[num];
                    }
                    i+=2;

                    //Reset temp and temp value.
                    t_size-=1;
                    while(t_size>=0){
                        temp[t_size]='\0';
                        t_size--;
                    }
                    temp_value='\0';
                }        
            }
            else if(polynomial[i]=='+'){
                sign=1;
                str[s_size]=sign;
            }
            else if(polynomial[i]=='-'){
                sign=-1;
                str[s_size]=sign;
            }
        }
        //Polynomial store double array is called above.

        //Four operation will do below.
        for(i=0;i<s_size;i++){
            j=1;
            k=1;
            
            while(k) {
                if(str[i+j]!=1 && str[i+j]!=-1 && s_size!=(i+j)) {
                   str[i]*=str[i+j];
                   j++;     
                } 
                else {
                   k=0;
                   result+=str[i];
                }
            }
            if(j!=1)
                i+=j-1;        
        } 
        //Writes result to evaluations.txt
        fprintf(fp,"%.2f\n",result);
        //Reset result.
        result=0;

        num++;
    }
    fclose(fp);
}
