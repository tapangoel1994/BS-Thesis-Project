#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
     char buffer[1024] ;
     char *record,*line;
     int i=0,j=0;
     int mat[1][1200];
     FILE *fstream = fopen("Density_3.25Noise_7.00.csv","r");
     if(fstream == NULL)
     {
	  printf("\n file opening failed ");
	  return -1 ;
     }
     while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL && i<1)
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