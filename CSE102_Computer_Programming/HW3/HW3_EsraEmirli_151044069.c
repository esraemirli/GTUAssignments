#include<stdio.h>
#define ROW 250
#define ROW_LENGTH 250
#define WORD_COUNT 5
void main(void){
    char words[WORD_COUNT][ROW_LENGTH]={ {"Xanthos"}, {"Patara"}, {"Myra"}, {"Arycanda"}, {"Phaselis"} };
    char letter[ROW][ROW_LENGTH];
    int words_size[WORD_COUNT];
    int  row_length[ROW];
    int size_word1,size_word2,size_word3,size_word4,size_word5;
    int i,j,k,w,row_size=0,max_length;
   
    //Find the size of words. 
    for(i=0;i<WORD_COUNT;i++){
        for(j=0;words[i][j]!='\0';){
            j++;
        }
        words_size[i]=j;
    }

    FILE *fp;
    fp = fopen("input.txt", "r");

    //Finds the size of rows.
    for(i=0;!feof(fp);i++){
        fscanf(fp,"%s",letter[row_size]); 
        row_size++;
    }
    row_size=row_size-1;
    fclose(fp);
    
    //Finds the row lenght for all row.
    for(i=0;i<row_size;i++){
        for(j=0;letter[i][j]!='\0';j++)
            row_length[i]++;
    }

    //Finds the maximum length size.
    max_length=row_length[0];        
    for(i=0;i<row_size;i++){
        if(row_length[i]>max_length)
            max_length=row_length[i];
    }

    fp=fopen("output.txt","w++");

    //Checks the all words.
    for(w=0;w<WORD_COUNT;w++) {
        //Words[w] is Horizontal or Not?
        for(i=0 ;i<row_size ;i++) {
           k=0;
           for(j=0 ;letter[i][j]!='\0' ;j++ ) {
           //Checking whether the letter matches the letter.
              if(letter[i][j]==words[w][k]){
                 //If equals the last letter,writes.
                 if(k==words_size[w]-1){
                     fprintf(fp,"%s  (%d,%d)  Horizontal\n",words[w],i+1,j-k+1);
                     break;
                 }
                 //If is not the last letter, increase.
                     else
                        k++;            
               }
                 //If not equal to word,it returns to the first letter of the word.
                 else
                     k=0;
           }
        }  
        //Words[w] is Vertical or Not?
        for(j=0;j<max_length;j++) {
            k=0;
            for(i=0; i<row_size; i++) {
                if(letter[i][j]==words[w][k]){
                    if(k==words_size[w]-1){
                       fprintf(fp,"%s  (%d,%d)  Vertical\n",words[w],i-k+1,j+1);
                       break;
                    }
                    else
                        k++;            
                }
                else
                    k=0;
           }   
        }
    }
    fclose(fp);
}
