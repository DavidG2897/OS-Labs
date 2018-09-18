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
long *result,*row_buff,*column_buff,**buffers,NUM_BUFFERS,n;	//temporal matrix declaration, allocation done when read requested
pthread_mutex_t *mutexes;		//Mutexes array

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
	for(i=0;i<ROW_SIZE;i++){
		row_buff[i]=matrix[index++];
	}
	return row_buff;
}

long *getColumn(int column, long *matrix){
	unsigned long i;
	
	return column_buff;	
}

int getLock(void){
	n=0;
	do{
		if(0==pthread_mutex_trylock(&mutexes[n])) break;
	}while(n++<NUM_BUFFERS-1);
	return n;
}

int releaseLock(int lock){
	int result;
	if(0==pthread_mutex_unlock(&mutexes[lock])){result=0;}else{result=-1;}
	return result;
}

long dotProduct(long *vec1,long *vec2){
	return 0;
}

long *multiply(long *matA,long *matB){
	int lock=getLock();
	releaseLock(lock);
	return 0;
}

int saveResultMatrix(long *result){
	return 0;
}

int main(void){
	unsigned long i;
	long *matA,*matB,*rowA1,*rowB0;
	matA=malloc(MATRIX_SIZE*sizeof(long));
	matB=malloc(MATRIX_SIZE*sizeof(long));
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

	rowA1=getRow(1,matA);
	rowB0=getRow(0,matB);
	return 0;
}


