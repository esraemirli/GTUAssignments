#include<stdio.h>
#define WIDTH 50
#define LENGTH 40

void source_code(int marked_width,int marked_length,int marked_x,int marked_y);
int user_guess();

int main(){
	user_guess();
}

void source_code(int marked_width,int marked_length,int marked_x,int marked_y){
	int i,j;

	//Draw rectangle and marked region in board.
	for(j=0;j<LENGTH;j++){
		for(i=0;i<WIDTH;i++){
			if(i>=marked_x-(marked_width/2) && i<=marked_x+(marked_width/2) && j>=marked_y-(marked_length/2) && 
				j<=marked_y+(marked_length/2) )
				//! Change "." to see marked region. !
				printf(". ");
			else
				printf(". ");
		}
		printf("\n");
	}	
}

int user_guess(){

	int a,b,count,trial=10;
	int i,j,k,x,y;
	
	//Center of the marked region. To change place marked region, change these values.
	int marked_x=35,marked_y=25;

	int marked_width=15,marked_length=20;
	int detector_width=20,detector_length=20;
	
	source_code(marked_width,marked_length,marked_x,marked_y);

	//Checks between detector and marked region relationship.
	if(detector_width<marked_width || detector_length<marked_length){
		printf("Detector should be bigger than the marked area. ");
		goto FINISH;
	}
	else	
		printf("Board Size: [%dx%d] Detector Size: [%dx%d]\n",WIDTH,LENGTH,detector_width,detector_length);


	//Your right to guess.
	for(trial=10;trial>0;){
		
		printf("Give a center of detector like x and y: ");
		scanf("%d",&x);
		scanf("%d",&y);
		count=0;
		//Checks whether or not intersection.If have intersection area count increase.

		for(i=marked_x-(marked_width/2);i<=marked_x+(marked_width/2) ;i++){

			for(j=marked_y-(marked_length/2);j<marked_y+(marked_length/2) ; j++ ){
		
				for(a=x-(detector_width/2);a<=x+(detector_width/2);a++){
		
					for(b=y-(detector_length/2);b<=y+(detector_length/2);b++){
						
						if(a==i && b==j)
							count++;		
					}
				}
			}
		}

		//Checks move is possible or not.
		if (x>WIDTH || y>LENGTH){
			printf("Error, move is not valid. Try again. Attempts remaining: %d\n",trial);
		}
		else{
				//If detector fully overlaps with the marked region, user wins.
				if(count==marked_width*marked_length && count==0) {
					printf("You win!");
					break;
				}

				//If there is no full or partial overlap, it is a miss.
				else if(count==0) {
					trial=trial-1;
					printf("You missed the target. Attempts remaining: %d\n",trial);
			 	}

				/* If there is a partial overlap, marked region is updated to the intersected area and user is notified about the shrinkage.Detector width and height are also decreased to half of their value for the next iteration. */
				else if(count>0 && count<marked_width*marked_length){
					trial=trial-1;
					marked_width=marked_width/2;
					marked_length=marked_length/2;
					detector_width=detector_width/2;
					detector_length=detector_length/2;

					// ! Delete the comment mark below to see the change in marked region. !
					//source_code(marked_width,marked_length,marked_x,marked_y);

					printf(" Partial hit! marked region was shirked to [%dx %d],detector was shirked to [%d x %d].Attempts remaining: %d\n",marked_width,marked_length,detector_width,detector_length,trial);
					
				}
		}

		//Your right to guess finished.
		if(trial==0)
			printf("You Lost!");
	}

	FINISH:
	return 0;
}

