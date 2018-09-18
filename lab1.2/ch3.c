#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sort(unsigned char list[],unsigned char size){
	int greater, lesser;
	int sorted[size];
	for(int i=0;i<size;i++){
		sorted[i]=list[i];
	}
	for(int i=0;i<size;i++){
		list[i]=sorted[i];
		for(int j=i;j>0;j--){
			if(list[j]<list[j-1]){
				greater=list[j-1];
				lesser=list[j];
				list[j]=greater;
				list[j-1]=lesser;
			}
		}
	}
}

unsigned char contains(unsigned char *a, unsigned char b, unsigned char size){
	for(unsigned char i=0;i<size;i++){
		if(b==a[i]){
			return 1;
		}
	}
	return 0;
}

int main(void){

	struct timespec t1,t2;
	clock_gettime(CLOCK_REALTIME,&t1);

	srand(time(NULL));					//Set rand seed to NULL
	unsigned char N=10;					//Given N integer
	unsigned char k=6;					
	unsigned char list[k];				//Members list
	unsigned char notMembers[N-k];
	unsigned char count=0, count2=0;
	unsigned char x;

	if(k==N){
		printf("INVALID PARAMETERS\nTerminating execution");
		return 0;
	}

	do{
		x=rand() % N;
		if(!contains(list,x,k)){
			list[count++]=x;
		}
	}while(count<k);

	count=0;		//reset counter
	sort(list,k);

	printf("Sorted members list: ");
	for(unsigned char i=0;i<k;i++){
		printf("%d ",list[i]);
	}
	printf("\n");

	for(unsigned char i=0;i<N;i++){
		if(i!=list[count]){
			notMembers[count2++]=i;
		}else{count++;}
	}

	printf("Not members list: ");
	for(int i=0;i<(N-k);i++){
		printf("%d ", notMembers[i]);
	}

	unsigned char randomNotMember = notMembers[rand() % ((N-k))];		//Constrained random number generation (0 to N-k-1)
	printf("\nRandom not member: %d\n",randomNotMember);				//Minus one for index coherence

	clock_gettime(CLOCK_REALTIME,&t2);
	printf("Exec time: %d ns",(t2.tv_nsec-t1.tv_nsec));
	return 0;
}
