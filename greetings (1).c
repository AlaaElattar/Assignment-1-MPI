#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <math.h>
#define FILENAME "joker.txt"
int main(int argc , char * argv[])
{
	int my_rank;
	int p;
	int source;
	int dest;
	int tag = 0;
	char message1[100000][10];
	char message2[100000][10];
	char str[60];
	char id[100000][10];
	char gr[100000][10];
	int datawork;
	int pass=0;
	FILE *fp;
	fp = fopen("/shared/joker.txt","r");
	MPI_Status status;

	MPI_Init( &argc , &argv );

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if( my_rank != 0)
	{
		dest = 0;
		int counter=0,j=0;
		MPI_Recv(&datawork,1, MPI_INT,dest, tag, MPI_COMM_WORLD, &status );
		while(counter!=datawork){
		    MPI_Recv(id[j], 100, MPI_CHAR,dest, tag, MPI_COMM_WORLD, &status );
			MPI_Recv(gr[j], 100, MPI_CHAR,dest, tag, MPI_COMM_WORLD, &status );
			int z=atoi(gr[j]);
			if((z)>=60){
				printf("%s %s %d\n",id[j],"Passed The Exam",my_rank);
				pass=pass+1;
			}
			else{
				printf("%s %s %d\n",id[j],"Failed. Please Repeat The Exam",my_rank);
			}
		     j=j+1;
		     counter=counter+1;
		}
		MPI_Send( &pass, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}
  
	else
	{
		if (fp == NULL){
            printf("Error! opening file");
        }
		int i=0;
		int countLine=0;
        while( fgets (str, 60, fp)!=NULL ) {
            sscanf(str,"%s %s",id[i],gr[i]);
            printf("%s \n",id[i]);
            printf("%s \n",gr[i]);
            i=i+1;
            countLine=countLine+1;
        }
		if(countLine%(p-1)!=0){
			datawork =((countLine/(p-1))+1);
		}
		else{
			datawork=(countLine/(p-1));
		}
		printf("%d",datawork);
		int j=0,counter=0,a;
		for(a=1;a<p;a++){
			if(datawork<=(countLine-j)){
				MPI_Send( &datawork, 1, MPI_INT, a, tag, MPI_COMM_WORLD);
			}
			else{
				datawork=(countLine-j);
				MPI_Send( &datawork, 1, MPI_INT, a, tag, MPI_COMM_WORLD);
			}
		    while(counter!=datawork){
		        MPI_Send( id[j], strlen(id[j])+1, MPI_CHAR, a, tag, MPI_COMM_WORLD);
		        MPI_Send( gr[j], strlen(gr[j])+1, MPI_CHAR, a, tag, MPI_COMM_WORLD);
		        j=j+1;
				if(id[j]==NULL){
					break;
				}
		        counter=counter+1;
		    }
		    counter=0;
		}
	int sum=0;
	for(i=1;i<p;i++){
		MPI_Recv(&pass,1, MPI_INT,i, tag, MPI_COMM_WORLD, &status );
		sum=sum+pass;
	}
	printf("%s %d %s %d \n","Total Number of students passed the exam is",sum,"out of",countLine);  
	}
	fclose(fp);
	MPI_Finalize();
	return 0;
}




