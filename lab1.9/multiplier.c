#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAT_A "./matA.dat"
#define MAT_B "./matB.dat"
#define ROW_SIZE  2000
#define COLUMN_SIZE 2000
#define MAX_THREADS 10
#define MATRIX_SIZE COLUMN_SIZE*ROW_SIZE

/*  Prototypes  */
long *readMatrix(char *filename);
long *getColumn(int column,long *matrix);
long *getRow(int row,long *matrix);
int getLock(void);
int releaseLock(int lock);
long dotProduct(long *vec1,long *vec2);
long *multiply(void *params);
int saveResultMatrix(long *result);

/*  Globals  */
long *result,*row_buff,*column_buff,**buffers,NUM_BUFFERS;
pthread_mutex_t *mutexes;
typedef struct{
	long *matA;
	long *matB;
	unsigned long n;
} args;

long *readMatrix(char *filename){
	FILE *file;
	unsigned long i=0;
	file = fopen(filename,"r");	//Open file passed as argument in READ mode
	while(fscanf(file,"%ld",&result[i++]) != EOF);
	fclose(file);
	return result;
}

long *getRow(int row, long *matrix){
	unsigned long i;	
	unsigned long index=ROW_SIZE*row;	//Range from 0 to 1999
	for(i=0;i<ROW_SIZE;i++) row_buff[i]=matrix[index++];
	return row_buff;
}

long *getColumn(int column, long *matrix){
	unsigned long i;
	for(i=0;i<COLUMN_SIZE;i++) column_buff[i]=matrix[i*COLUMN_SIZE+column];
	return column_buff;
}

int getLock(void){
	int i=0;
	do{
		if(0==pthread_mutex_trylock(&mutexes[i])) return i;
	}while(i++<NUM_BUFFERS-1);
	return -1;
}

int releaseLock(int lock){
	int result;
	if(0==pthread_mutex_unlock(&mutexes[lock])){result=0;}else{result=-1;}
	return result;
}

long dotProduct(long *vec1,long *vec2){
	long result=0;
	unsigned long i;
	for(i=0;i<ROW_SIZE;i++) result+=vec1[i]*vec2[i];
	return result;
}

long *multiply(void *params){
	args *input=params;
	int lock,i;
	while(-1==(lock=getLock()));
	/*params->n++;
	for(i=0;i<ROW_SIZE;i++){
		buffers[lock][i]=dotProduct();
	}*/
	printf("%d\n",lock);
	releaseLock(lock);
	pthread_exit(0);
}

int saveResultMatrix(long *result){
	return 0;
}

int main(void){
	unsigned long i=0;
	long *matA,*matB,*row1,*column1;
	pthread_attr_t attr;
	pthread_t *threads;
	args parameters;

	/*  Memory allocations  */
	threads=malloc(MAX_THREADS*sizeof(pthread_t));	//One thread per column
	matA=malloc(MATRIX_SIZE*sizeof(long));
	matB=malloc(MATRIX_SIZE*sizeof(long));
	row1=malloc(ROW_SIZE*sizeof(long));
	column1=malloc(COLUMN_SIZE*sizeof(long));
	result=malloc(MATRIX_SIZE*sizeof(long));
	row_buff=malloc(ROW_SIZE*sizeof(long));
	column_buff=malloc(COLUMN_SIZE*sizeof(long));

	pthread_attr_init(&attr);		//Default thread attributes

	printf("Input desired number of buffers: ");
	fflush(stdout);
	scanf("%d",&NUM_BUFFERS);

	/*
 	Buffers init
	First dimension of buffers 2D array size is allocated as NUM_BUFFERS long arrays
	Second dimension of buffers 2D array size is allocated as ROW_SIZE sized long arrays
	*/
	buffers=malloc(NUM_BUFFERS*sizeof(long*)); 
	for(i=0;i<NUM_BUFFERS;i++){
		buffers[i]=malloc(ROW_SIZE*sizeof(long));
	}
	/*
	Mutexes are related one on one with buffers (buffers[0] is associated with mutex[0] and so on)
	*/
	mutexes=malloc(NUM_BUFFERS*sizeof(pthread_mutex_t)); 
	
	//Temporal matrix fill
	unsigned long j;
	for(j=0;j<MATRIX_SIZE;j++){
		matA[j]=1;
	}
	for(j=0;j<MATRIX_SIZE;j++){
		matB[j]=2;
	}
	//

	//memcpy(matA,readMatrix(MAT_A),MATRIX_SIZE*sizeof(long));
	//memcpy(matB,readMatrix(MAT_B),MATRIX_SIZE*sizeof(long));

	/*memcpy(row1,getRow(0,matA),ROW_SIZE*sizeof(long));
	memcpy(column1,getColumn(0,matB),COLUMN_SIZE*sizeof(long));*/

	parameters.matA=matA;	
	parameters.matB=matB;	

	do{
		for(j=0;j<MAX_THREADS;j++){			//One thread per column, MAX_THREADS at a time
			pthread_create(&threads[j],&attr,multiply,(void *) &parameters);
		}

		for(j=0;j<MAX_THREADS;j++){
			pthread_join(threads[j],NULL);		//Join all threads		
		}

	}while(i++<COLUMN_SIZE/MAX_THREADS);

	//Free allocated memory
	free(mutexes);
	free(matA);
	free(matB);
	free(result);
	free(row_buff);
	free(column_buff);
	return 0;
}


