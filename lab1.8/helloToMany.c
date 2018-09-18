#include <pthread.h>
#include <stdio.h>

typedef struct{
	long id;
	int iteration;
} args;

void *runner(void *param); 

main(int argc, char *argv[])
{
	
	int i, scope,NUM_THREADS;
	printf("Input number of threads: ");
	fflush(stdout);
	scanf("%d",&NUM_THREADS);
	args args_array[NUM_THREADS];
	pthread_t tid[NUM_THREADS]; 	/* the thread identifier */
	pthread_attr_t attr; 		/* set of attributes for the thread */

	srand(NULL);

	pthread_attr_init(&attr);

	if (pthread_attr_getscope(&attr,&scope) != 0)
		fprintf(stderr, "Unable to get scheduling scope.\n");
	else {
		if (scope == PTHREAD_SCOPE_PROCESS)
			printf("PTHREAD_SCOPE_PROCESS\n");
		else if (scope == PTHREAD_SCOPE_SYSTEM)
			printf("PTHREAD_SCOPE_SYSTEM\n");
		else 
			fprintf(stderr,"Illegal scope value.\n");
	}			
	
	if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM) != 0)
		printf("unable to set scheduling policy.\n");

	for (i = 0; i < NUM_THREADS; i++){
		args_array[i].id=rand()%NUM_THREADS;	//This is so ID and Iteration are not the same
		args_array[i].iteration=i;
		pthread_create(&tid[i],&attr,runner,(void *) &args_array[i]);
		printf("I am the main thread. Created a new thread (%ld) in iteration %d\n",(void *)args_array[i].id,args_array[i].iteration);
		if(0==(i+1)%5) sleep(1);
	}
	
	for(i = 0; i< NUM_THREADS; i++)
		pthread_join(tid[i], NULL);

}

void *runner(void *param) 
{
	args *a=param;
	printf("I am thread %d. I was created in iteration %ld\n",a->id,a->iteration);
	pthread_exit(0);
}

