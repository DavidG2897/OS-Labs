#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define CHAIR_COUNT 5
#define SEED 1234567890

//Prototypes
void *thread(void *params);
void init_philosophers(void);
void test(int i);
void pickup_forks(int philosopher_number);
void return_forks(int philosopher_number);

//Globals
enum {THINKING,EATING} philosophers[CHAIR_COUNT];
pthread_t threads[CHAIR_COUNT];
pthread_mutex_t mutexes[CHAIR_COUNT];
pthread_cond_t cond_vars[CHAIR_COUNT];
pthread_attr_t attr;

void init_philosophers(void){
	unsigned char i;
	for(i=0;i<CHAIR_COUNT;i++){
		pthread_mutex_init(&mutexes[i],NULL);
		pthread_cond_init(&cond_vars[i],NULL);
		philosophers[i] = THINKING;
	}
	for(i=0;i<CHAIR_COUNT;i++) pthread_create(&threads[i],&attr,thread,(void *) i);
	for(i=0;i<CHAIR_COUNT;i++) pthread_join(threads[i],NULL);
}

void test(int i){
	unsigned char left=(i+(CHAIR_COUNT-1))%CHAIR_COUNT;
	unsigned char right=(i+1)%CHAIR_COUNT;
	//If philosopher can get both forks...
	if(THINKING==philosophers[left]==philosophers[i]==philosophers[right]){
		//.. it starts eating
		philosophers[i]=EATING;
		pthread_cond_signal(&cond_vars[i]);
	}
}

void pickup_forks(int philosopher_number){
	printf("picking up forks\n");
	test(philosopher_number);
}

void return_forks(int philosopher_number){
	unsigned char left=(philosopher_number+(CHAIR_COUNT-1))%CHAIR_COUNT;
	unsigned char right=(philosopher_number+1)%CHAIR_COUNT;
	test(left);
	test(right);
}

void *thread(void *params){
	int philosopher = (int) params;
	pthread_mutex_lock(&mutexes[philosopher]);
	unsigned char left=(philosopher+(CHAIR_COUNT-1))%CHAIR_COUNT;
	unsigned char right=(philosopher+1)%CHAIR_COUNT;
	printf("Philosopher %d is now trying to eat...",philosopher);
	pickup_forks(philosopher);
	while(EATING!=philosophers[philosopher]){
		pthread_cond_wait(&cond_vars[philosopher],&mutexes[philosopher]);
	}
	printf("\tPhilosophers %d and %d are not eating, %d is eating now!\n",left,right,philosopher);
	sleep((rand() % 3)+1);		//Eating...
	philosophers[philosopher]=THINKING;
	printf("Philosopher %d (%d) finished eating! Returning forks...", philosopher,philosophers[philosopher]);
	return_forks(philosopher);
	printf("philosopher %d returned its forks!\n",philosopher);
}

int main(void){
	srand(SEED);
	pthread_attr_init(&attr);
	init_philosophers();
	printf("All philosophers finished eating!\n");
	return 0;
}
