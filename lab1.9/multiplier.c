#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAT_A "./matA.dat"
#define MAT_B "./matB.dat"
#define ROW_SIZE  2000
#define COLUMN_SIZE 2000
#define MATRIX_SIZE COLUMN_SIZE*ROW_SIZE

/*  Prototypes  */
long *readMatrix(char *filename);
long *getColumn(int column,long *matrix);
long *getRow(int row,long *matrix);
int getLock(void);
int releaseLock(int lock);
long dotProduct(long *vec1,long *vec2);
long *multiply(long *matA,long *matB);
int saveResultMatrix(long *result);

/*  Globals  */
long *result,*row_buff,*column_buff,**buffers,NUM_BUFFERS;
pthread_mutex_t *mutexes;

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

long *multiply(long *matA,long *matB){
	int lock=getLock();
	printf("%d\n",lock);
	long *rowA=getRow(0,matA);
	releaseLock(lock);
	return 0;
}

int saveResultMatrix(long *result){
	return 0;
}

int main(void){
	unsigned long i;
	long *matA,*matB,*row1,*column1;
	/*  Memory allocations  */
	matA=malloc(MATRIX_SIZE*sizeof(long));
	matB=malloc(MATRIX_SIZE*sizeof(long));
	row1=malloc(ROW_SIZE*sizeof(long));
	column1=malloc(COLUMN_SIZE*sizeof(long));
	result=malloc(MATRIX_SIZE*sizeof(long));
	row_buff=malloc(ROW_SIZE*sizeof(long));
	column_buff=malloc(COLUMN_SIZE*sizeof(long));

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

	memcpy(matA,readMatrix(MAT_A),MATRIX_SIZE*sizeof(long));
	memcpy(matB,readMatrix(MAT_B),MATRIX_SIZE*sizeof(long));

	//Free allocated memory
	free(mutexes);
	free(matA);
	free(matB);
	free(result);
	free(row_buff);
	free(column_buff);
	return 0;
}


