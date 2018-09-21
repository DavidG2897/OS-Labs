#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

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
long *multiply(long *matA, long *matB);
int saveResultMatrix(long *result);
void *thread_runner(void *params);

/*  Globals  */
long *result,**buffers,*row_buff,*column_buff,NUM_BUFFERS,row_count=-1;
pthread_mutex_t *mutexes;
pthread_mutex_t row_mutex;
pthread_t *threads;
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
	if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM) != 0) printf("unable to set scheduling policy.\n");
	parameters.matA=matA;
	parameters.matB=matB;
	do{
		for(i=0;i<NUM_BUFFERS;i++){
			pthread_create(&threads[i],&attr,thread_runner,NULL);
		}
		for(i=0;i<NUM_BUFFERS;i++){
			pthread_join(threads[i],NULL);
		}
	}while(j++<(ROW_SIZE/NUM_BUFFERS)-1);
	return result;
}

void *thread_runner(void *params){
	int lock;
	unsigned long i;
	long *row,*column;
	while(-1==(lock=getLock()));	//Wait for a buffer to be unlocked
	pthread_mutex_lock(&row_mutex);
	row_count++;
	row=malloc(ROW_SIZE*sizeof(long));
	column=malloc(COLUMN_SIZE*sizeof(long));
	memcpy(row,getRow(row_count,parameters.matA),ROW_SIZE*sizeof(long));
	for(i=0;i<COLUMN_SIZE;i++){
		memcpy(column,getColumn(i,parameters.matB),ROW_SIZE*sizeof(long));
		buffers[lock][i]=dotProduct(row,column);
	}
	for(i=0;i<COLUMN_SIZE;i++){
		result[row_count*ROW_SIZE+i]=buffers[lock][i];
	}
	free(row);
	free(column);
	pthread_mutex_unlock(&row_mutex);
	releaseLock(lock);
	pthread_exit(0);
}

int saveResultMatrix(long *res){
	int i;
	FILE *f=fopen("result.dat","w");
	for(i=0;i<MATRIX_SIZE;i++) fprintf(f,"%ld\n",res[i]);
	fclose(f);	
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
	threads=malloc(NUM_BUFFERS*sizeof(pthread_t));

	printf("Input desired number of buffers: ");
	fflush(stdout);
	scanf("%d",&NUM_BUFFERS);

	if(NUM_BUFFERS>=4) NUM_BUFFERS=4;

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
	saveResultMatrix(res);
	//}
	//Free allocated memory
	free(mutexes);
	free(matA);
	free(matB);
	free(result);
	free(res);
	free(buffers);
	free(mutexes);
	free(row_buff);
	free(column_buff);
	free(threads);
	return 0;
}
