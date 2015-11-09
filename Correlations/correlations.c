/*Program to calculate spatial correlation between velocity fluctuations at a given time-step.
 *Read position and orientation data from a csv file and use it to compute distance matrix. From the distance matrix, compute average correlatios
 * at various distance intervals.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void read_file(FILE *file);

int main()
{
     char buffer[1200] ;
     char *record,*line;
     int i=0,j=0;
     float mat[2][1200];
     FILE *fstream = fopen("test.csv","r");
     if(fstream == NULL)
     {
	  printf("\n file opening failed ");
	  return -1 ;
     }
     while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
     {
	  record = strtok(line,",");
	  while(record != NULL)
	  {
	       printf("record : %s",record) ;    //here you can put the record into the array as per your requirement.
	       mat[i][j++] = atoi(record) ;
	       record = strtok(line,",");
	  }
	  ++i ;
	  
     }
     
     return 0 ;
     
     
}