#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#define TIME_SLICE 15
#define TASKS_LIMIT 10

char first=-1,finish=0;
unsigned char cnt=0;

enum STATES {IDLE,READY,RUN};		//Ignore WAIT state, no IO
typedef struct{
	unsigned char id;
	unsigned char burst_time;
	unsigned char priority;
	unsigned char activation_time;	//Activation time, could get it from system but it will always be sorted
	unsigned long wait_time;
	enum STATES state;
} Task;
Task tasks[TASKS_LIMIT];

void run(void){
	tasks[first].wait_time=cnt;
	printf("Running Task ID: %d ACTIVATION_TIME: %d STATE: RUN WAIT_TIME: %d\n",tasks[first].id,tasks[first].activation_time,tasks[first].wait_time);
	unsigned char i=0;
	do{
		cnt++;
	}while(i++<tasks[first].burst_time-1);
	tasks[first].state=IDLE;
}

void RR_run(){
	unsigned char size;
	tasks[first].wait_time=cnt;
	printf("Running Task ID: %d ACTIVATION_TIME: %d BURST_TIME:%d WAIT_TIME: %d\n",tasks[first].id,tasks[first].activation_time,tasks[first].burst_time,tasks[first].wait_time);
	unsigned char i=0,timeout=0;
	static unsigned char n;
	do{
		if(i==tasks[first].burst_time) break;
		cnt++;
	}while(i++<TIME_SLICE-1);
	if(i==TIME_SLICE){
		tasks[first].burst_time-=TIME_SLICE;
		tasks[first].state=READY;
		Task tmp=tasks[first];
		for(i=first;i<TASKS_LIMIT;i++){
			tasks[i]=tasks[i+1];
			size=i;
		}
		tasks[TASKS_LIMIT-1]=tmp;	
	}else{
		tasks[first].state=IDLE;	
	}
}

void sort(Task list[],unsigned char size,unsigned char policy){
	Task greater, lesser;
	unsigned char i,j;
	Task sorted[size];
	for(i=0;i<size;i++){
		sorted[i]=list[i];
	}
	for(i=0;i<size;i++){
		list[i]=sorted[i];
		for(j=i;j>0;j--){
			switch(policy){
				case 1:
					if(list[j].activation_time<list[j-1].activation_time){
						greater=list[j-1];
						lesser=list[j];
						list[j]=greater;
						list[j-1]=lesser;
					}
					break;
				case 2:
					if(list[j].burst_time>list[j-1].burst_time){
						greater=list[j-1];
						lesser=list[j];
						list[j]=greater;
						list[j-1]=lesser;
					}
					break;
				case 3:
					if(list[j].priority<list[j-1].priority){
						greater=list[j-1];
						lesser=list[j];
						list[j]=greater;
						list[j-1]=lesser;
					}
					break;
				case 4:
					if(list[j].activation_time<list[j-1].activation_time){
						greater=list[j-1];
						lesser=list[j];
						list[j]=greater;
						list[j-1]=lesser;
					}
					break;
				default:
					break;
			}
		}
	}
}

void fcfs(void){
	unsigned char i;
	static unsigned char sorted;
	first=-1;//Assume no task running
	sort(tasks,TASKS_LIMIT,1);
	if(!sorted){
		printf("-------------------FCFS Scheduling-------------------------\n");
		for(i=0;i<TASKS_LIMIT;i++){
			printf("Task ID: %d ACTIVATION_TIME: %d STATE: %s BURST_TIME: %d\n",tasks[i].id,tasks[i].activation_time,(!tasks[i].state) ? "IDLE" : "READY",tasks[i].burst_time);
		}
		sorted=1;
	}
	for(i=0;i<TASKS_LIMIT;i++){
		if(tasks[i].state==READY){//Already sorted by activation time, first READY task is selected to RUN
			first=i;
			break;	
		}
	}
	printf("-----------------------------------------------------------\n");	
	if(first<0){
		printf("No more tasks to run...finishing\n");
		finish=1;
	}else{
		tasks[first].state=RUN;
		run();
	}
}

void priority_scheduling(void){
	unsigned char i;
	static unsigned char sorted;
	first=-1;//Assume no task running
	sort(tasks,TASKS_LIMIT,3);
	if(!sorted){
		printf("-------------------Priority Scheduling-------------------------\n");
		for(i=0;i<TASKS_LIMIT;i++){
			printf("Task ID: %d PRIORITY: %d BURST_TIME: %d STATE: %s\n",tasks[i].id,tasks[i].priority,tasks[i].burst_time,(!tasks[i].state) ? "IDLE" : "READY");
		}
		sorted=1;	
	}
	for(i=TASKS_LIMIT-1;i>0;i--){
		if(tasks[i].state==READY){//Already sorted by piority, first READY task is selected to RUN
			first=i;
			break;	
		}
	}
	printf("---------------------------------------------------------------\n");
	if(first<0){
		printf("No more tasks to run...finishing\n");
		finish=1;
	}else{
		tasks[first].state=RUN;
		run();
	}
}

void sjf(void){
	unsigned char i;
	static unsigned char sorted;
	first=-1;//Assume no task running
	sort(tasks,TASKS_LIMIT,2);
	if(!sorted){
		printf("-------------------SJF Scheduling-------------------------\n");
		for(i=0;i<TASKS_LIMIT;i++){
			printf("Task ID: %d BURST_TIME: %d STATE: %s\n",tasks[i].id,tasks[i].burst_time,(!tasks[i].state) ? "IDLE" : "READY");
		}
		sorted=1;	
	}
	for(i=TASKS_LIMIT-1;i>0;i--){
		if(tasks[i].state==READY){//Already sorted by burst time, first READY task is selected to RUN
			first=i;
			break;	
		}
	}
	printf("----------------------------------------------------------\n");
	if(first<0){
		printf("No more tasks to run...finishing\n");
		finish=1;
	}else{
		tasks[first].state=RUN;
		run();
	}
}

void round_robin(void){
	unsigned char i;
	static unsigned char sorted,flag;
	first=-1;//Assume no task running
	if(!sorted){
		sort(tasks,TASKS_LIMIT,4);
		printf("-------------------Round-robin Scheduling-------------------------\n");
		for(i=0;i<TASKS_LIMIT;i++){
			printf("Task ID: %d ACTIVATION_TIME: %d STATE: %s BURST_TIME: %d\n",tasks[i].id,tasks[i].activation_time,(!tasks[i].state) ? "IDLE" : "READY",tasks[i].burst_time);
		}
		sorted=1;
	}
	printf("--------------------------------------------------------------------------\n");
	for(i=0;i<TASKS_LIMIT;i++){
		if(tasks[i].state==READY){
			first=i;
			break;		
		}
	}
	if(first<0){
		printf("No more tasks to run...finishing\n");
		finish=1;
	}else{
		tasks[first].state=RUN;
		RR_run();
	}
}

void init_tasks(void){
	unsigned char i;
	for(i=0;i<TASKS_LIMIT;i++){
		tasks[i].id=i;
		tasks[i].burst_time=(rand() % 50)+1;
		tasks[i].priority=(rand() % 50);
		tasks[i].wait_time=0;
		tasks[i].state=(rand() % 2); //Randomize autostart
		if(!tasks[i].state){
			tasks[i].activation_time=0;
		}else{
			tasks[i].activation_time=(rand() % 10000);					
		}
	}
}

int main(void){
	srand(time(NULL));
	unsigned char run=1;
	while(run){
		char select;
		printf("Select scheduling policy:\n1) FCFS\n2) SJF\n3) Priority scheduling\n4) Round-robin scheduling\n> ");
		scanf("%c",&select);
		init_tasks();
		switch(select){
			case '1':
				while(!finish) fcfs();
				run=0;
				break;
			case '2':
				while(!finish) sjf();
				run=0;
				break;
			case '3':
				while(!finish) priority_scheduling();
				run=0;
				break;
			case '4':
				while(!finish) round_robin();
				run=0;
				break;
			default:
				printf("Please select a valid option\n");
				break;
		}
	}
	return 0;
}
