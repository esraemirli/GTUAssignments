#include<stdio.h>
#define ROW 1000
#define COLUMN 1000

double chunk_function(double num[1000],int file_size);
int main(){
 	double num[1000];
	int size=0;

	// Open file and save the chunk array. 
	FILE *fp;
    fp = fopen("input.txt", "r");

	//Checks size of array.
	while(!feof(fp)) { 
		fscanf(fp,"%lf",&num[size]);
		size++;
	}
	fclose(fp);

	chunk_function(num,size);
   
}
double chunk_function(double num[1000],int file_size){
	
	double chunk[ROW][COLUMN];
	double copy_chunk[ROW];
	int size[COLUMN];
	int index[ROW];
	int num_chunk;
	double total,temp=0;
	int count=0;
	int i,j,k,l,a,b;
	
	//Delete 0 and count the number of chunks."j" is number.
	j=0;
	for(i=0,k=0;i<file_size;i++) {
		if(num[i-1]!=0 && num[i]==0 && num[i+1]==0 && num[i+2]==0){
			JUMP:
			size[j]=k;
			i+=2;
			j++;
			k=0;
		}
		else {
			chunk[j][k]=num[i];
			k++;
		}
		if(i==file_size-1)
			goto JUMP;
	}
	num_chunk=j;

	//Calculates average.
	for(a=0;a<num_chunk;a++) {
		total=0;
		for(b=0;b<size[a];b++)
			total+=chunk[a][b];
		chunk[a][size[a]]=total/size[a];
	}	

	//Copies to other chunk aray.
	for(i=0; i<num_chunk; i++) 
		copy_chunk[i]=chunk[i][size[i]];


	//Sortes the averages.
	for(i=0; i<num_chunk; i++) {
		for(l=i+1 ;l<num_chunk ;l++) {
			if(chunk[i][size[i]]>chunk[l][size[l]]) {
				temp=chunk[l][size[l]];
				chunk[l][size[l]]=chunk[i][size[i]];
				chunk[i][size[i]]=temp;
			}
		}
	}

	//Intersection area is original index of the averages so checks intersection area.
	for(k=0;k<num_chunk;k++){
		count=0;
		for(l=0;l<num_chunk;l++){
			if(chunk[k][size[k]]==copy_chunk[l]) {
				index[k]=count;
			}
			else
				count+=1;
		}
	}
	
	//Write to file using index.
	FILE *fp;
	fp= fopen("output.txt","w++");

		for(i=0;i<num_chunk;i++) {
			a=index[i];
			for(b=0;b<size[a];b++) {
				fprintf(fp,"%.1lf ",chunk[a][b]);
			}
			fprintf(fp,"\n");
		}

	fclose(fp);	
}
