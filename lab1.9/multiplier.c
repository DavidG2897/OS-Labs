#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#define MAT_A "./matA.dat"
#define MAT_B "./matB.dat"
#define ROW_SIZE  2000
#define COLUMN_SIZE 2000
#define MAX_THREADS 20
#define MATRIX_SIZE COLUMN_SIZE*ROW_SIZE

/*  Prototypes  */
long *readMatrix(char *filename);
long *getColumn(int column,long *matrix);
long *getRow(int row,long *matrix);
int getLock(void);
int releaseLock(int lock);
long dotProduct(long *vec1,long *vec2);
long *multiply(long *matA, long *matB);
int saveResultMatrix(long *result);
void *thread_runner(void *params);

/*  Globals  */
long *result,**buffers,*row_buff,*column_buff,NUM_BUFFERS;
pthread_mutex_t *mutexes;
pthread_t threads[MAX_THREADS];
typedef struct{
	long *matA,*matB;
} args;
args parameters;

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
		if(0==pthread_mutex_trylock(&mutexes[i])){
			return i;
		}
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
	unsigned long i,j=0,n=0;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	parameters.matA=matA;
	parameters.matB=matB;
	do{	
		for(i=0;i<MAX_THREADS;i++){	
			pthread_create(&threads[i],&attr,thread_runner,(void *) n);
			n++;
		}
		for(i=0;i<MAX_THREADS;i++){
			pthread_join(threads[i],NULL);
		}
	}while(j++<(ROW_SIZE/MAX_THREADS)-1);
	return result;
}

void *thread_runner(void *params){
	int lock;
	unsigned long i;
	long *row,*column;
	unsigned long n=(unsigned long)params;
	while(-1==(lock=getLock()));	//Wait for a mutex to be unlocked
	printf("n: %ld lock: %ld\n",n,lock);
	row=malloc(ROW_SIZE*sizeof(long));
	column=malloc(COLUMN_SIZE*sizeof(long));
	memcpy(row,getRow(n,parameters.matA),ROW_SIZE*sizeof(long));
	for(i=0;i<COLUMN_SIZE;i++){
		memcpy(column,getColumn(i,parameters.matB),ROW_SIZE*sizeof(long));
		if(n==i) printf("n: %ld %ld\n",n,dotProduct(row,column));
		buffers[lock][i]=dotProduct(row,column);
		if(n==i) printf("n: %ld %ld\n",n,buffers[lock][i]);
	}
	for(i=0;i<COLUMN_SIZE;i++){
		result[n*ROW_SIZE+i]=buffers[lock][i];
	}
	free(row);
	free(column);
	releaseLock(lock);
	pthread_exit(0);
}

int saveResultMatrix(long *result){
	return 0;
}

int main(void){
	unsigned long i=0;
	long *matA,*matB,*res;
	pthread_mutexattr_t attr;

	/*  Memory allocations  */
	matA=malloc(MATRIX_SIZE*sizeof(long));
	matB=malloc(MATRIX_SIZE*sizeof(long));
	res=malloc(MATRIX_SIZE*sizeof(long));
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
	//Temporal matrix fill
	/*unsigned long j;
	for(j=0;j<MATRIX_SIZE;j++){
		matA[j]=1;
	}
	for(j=0;j<MATRIX_SIZE;j++){
		matB[j]=1;
	}*/
	//

	memcpy(matA,readMatrix(MAT_A),MATRIX_SIZE*sizeof(long));
	memcpy(matB,readMatrix(MAT_B),MATRIX_SIZE*sizeof(long));

	/*
	printf("%ld\n",matA[0]);
	printf("%ld\n",matB[0]);
	
	memcpy(row_buff,getRow(0,matA),ROW_SIZE*sizeof(long));
	memcpy(column_buff,getColumn(0,matB),COLUMN_SIZE*sizeof(long));

	printf("%ld\n",dotProduct(row_buff,column_buff));
	*/

	memcpy(res,multiply(matA,matB),MATRIX_SIZE*sizeof(long));
	
	//unsigned long j;
	//for(j=0;j<MATRIX_SIZE;j++){
	printf("%ld\n",res[0]);
	//}
	//Free allocated memory
	free(mutexes);
	free(matA);
	free(matB);
	free(result);
	free(row_buff);
	free(column_buff);
	return 0;
}


