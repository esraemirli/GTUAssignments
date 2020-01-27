#include<stdio.h>
void main(void){
    char letter[27]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};
    
    char bi_gram[26][28][2];
    char ch1,ch2;

    double count[6][728];
    double total[6];
    double frequency[6][728];
    double dissimilarity[5];
    double difference;

    int i,j,index;

    //Filling bi-gram with all possibilities 
    for(i=0;i<26;i++){
        for(j=0;j<27;j++){
            bi_gram[i][j][0]=letter[i];
            bi_gram[i][j][1]=letter[j];
        }
        bi_gram[i][27][0]=' ';
        bi_gram[i][27][1]=letter[i];
    }

    FILE *fp;
    //Calculating each bi-grams how many times repeated for language 1
    fp=fopen("language_1.txt","r");    
    i=0;
    while(!feof(fp) && fp!='\0'){
        if(i==0){
            ch1=fgetc(fp);
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        else{
            ch1=ch2;
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        for(i=0;i<26;i++){
            for(j=0;j<28;j++){
                if(ch1==bi_gram[i][j][0]){
                    if(ch2==bi_gram[i][j][1]){
                        index=(i*28)+j;
                        count[0][index]+=1;
                    }
                }
            }
        }
    }
    fclose(fp); 

    //Calculating each bi-grams how many times repeated for language 2
    fp=fopen("language_2.txt","r");  
    i=0;
    while(!feof(fp) && fp!='\0'){
        if(i==0){
            ch1=fgetc(fp);
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
                
        }
        else{
            ch1=ch2;
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        for(i=0;i<26;i++){
            for(j=0;j<28;j++){
                if(ch1==bi_gram[i][j][0]){
                    if(ch2==bi_gram[i][j][1]){
                        index=(i*28)+j;
                        count[1][index]+=1;
                    }
                }
            }
        }
    }
    fclose(fp);

    //Calculating each bi-grams how many times repeated for language 3
    fp=fopen("language_3.txt","r"); 
    i=0;
    while(!feof(fp) && fp!='\0'){
        if(i==0){
            ch1=fgetc(fp);
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        else{
            ch1=ch2;
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        for(i=0;i<26;i++){
            for(j=0;j<28;j++){
                if(ch1==bi_gram[i][j][0]){
                    if(ch2==bi_gram[i][j][1]){
                        index=(i*28)+j;
                        count[2][index]+=1;
                    }
                }
            }
        }
    }
    fclose(fp);

    //Calculating each bi-grams how many times repeated for language 4
    fp=fopen("language_4.txt","r");  
    i=0;
    while(!feof(fp) && fp!='\0'){
        if(i==0){
            ch1=fgetc(fp);
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        else{
            ch1=ch2;
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        for(i=0;i<26;i++){
            for(j=0;j<28;j++){
                if(ch1==bi_gram[i][j][0]){
                    if(ch2==bi_gram[i][j][1]){
                        index=(i*28)+j;
                        count[3][index]+=1;
                    }
                }
            }
        }
    }
    fclose(fp);

    //Calculating each bi-grams how many times repeated for language 5
    fp=fopen("language_5.txt","r");   
    i=0;
    while(!feof(fp) && fp!='\0'){
        if(i==0){
            ch1=fgetc(fp);
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        else{
            ch1=ch2;
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        for(i=0;i<26;i++){
            for(j=0;j<28;j++){
                if(ch1==bi_gram[i][j][0]){
                    if(ch2==bi_gram[i][j][1]){
                        index=(i*28)+j;
                        count[4][index]+=1;
                    }
                }
            }
        }
    }
    fclose(fp);

    //Calculating each bi-grams how many times repeated for language x
    fp=fopen("language_x.txt","r");   
    i=0;
    while(!feof(fp) && fp!='\0'){
        if(i==0){
            ch1=fgetc(fp);
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        else{
            ch1=ch2;
            ch2=fgetc(fp);
            if(ch1=='\n')
                ch1=' ';
            if(ch2=='\n')
                ch2=' ';
        }
        for(i=0;i<26;i++){
            for(j=0;j<28;j++){
                if(ch1==bi_gram[i][j][0]){
                    if(ch2==bi_gram[i][j][1]){
                        index=(i*28)+j;
                        count[5][index]+=1;
                    }
                }
            }
        }
    }
    fclose(fp);
   
    //Calculating frequency   
    for(i=0;i<6;i++) {
        total[i]=0;
        for(j=0;j<728;j++)
            total[i]+=count[i][j];
    }
    for(i=0;i<6;i++){
        for(j=0;j<728;j++)
            frequency[i][j]= count[i][j]/total[i];
    }
  
    //Calculating dissimilarity     
    for(i=0;i<5;i++){
        dissimilarity[i]=0;
        for(j=0;j<727;j++){
            difference=frequency[i][j]-frequency[5][j];
            //Absolute value
            if(difference<0)
                difference*=-1;
            dissimilarity[i]+=difference;            
        }
        printf("%lf\n",dissimilarity[i]);
    }
}
