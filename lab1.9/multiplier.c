#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LINE 10	//Maximum number of characters per line of file
#define PATH_MAT_A "matA.dat"
#define PATH_MAT_B "matB.dat"
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
long *result,**buffers,NUM_BUFFERS;	//temporal matrix declaration, allocation done when read requested


long *readMatrix(char *filename){
	FILE *file;
	unsigned long i=0;
	result=malloc(MATRIX_SIZE*sizeof(long));
	file = fopen(filename,"r");	//Open file passed as argument in READ mode
	while(fscanf(file,"%ld",&result[i++]) != EOF);
	fclose(file);
	return result;
}

long *getRow(int row, long *matrix){
	/*
	* Need to add mutexes so that getRow fetches available buffer and returns data stored in said buffer
	* Also so it locks it before writing to it an releases it after write has finished
	*/
	unsigned long i;	
	unsigned long index=ROW_SIZE*row;	//Range from 0 to 1999
	for(i=0;i<ROW_SIZE;i++){
		buffers[0][i]=matrix[index++];
	}	
	return buffers[0];
}

long *getColumn(int column, long *matrix){
	return 0;	
}

int getLock(void){
	return 0;
}

int releaseLock(int lock){
	return 0;
}

long dotProduct(long *vec1,long *vec2){
	return 0;
}

long *multiply(long *matA,long *matB){
	return 0;
}

int saveResultMatrix(long *result){
	return 0;
}

int main(void){
	unsigned long i,j;
	long *matA,*matB,*rowA1;
	printf("Input desired number of buffers: ");
	fflush(stdout);
	scanf("%d",&NUM_BUFFERS);

	/* 	Buffers init
		First dimension of buffers 2D array size is allocated as NUM_BUFFERS long arrays
		Second dimension of buffers 2D array size is allocated as ROW_SIZE sized long arrays
	*/
	buffers=malloc(NUM_BUFFERS*sizeof(long*)); 
	for(i=0;i<NUM_BUFFERS;i++){
		buffers[i]=malloc(ROW_SIZE*sizeof(long));
	}

	matA=readMatrix(PATH_MAT_A);
	matB=readMatrix(PATH_MAT_B);
	
	rowA1=getRow(1,matA);

	return 0;
}


