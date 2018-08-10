#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (void){

	struct timespec t1,t2;
	clock_gettime(CLOCK_REALTIME,&t1);

	unsigned char flag=0;
	unsigned char g [] = "google";
	unsigned char size = (sizeof(g)/sizeof(g[0]))-1;

	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			if((g[i]==g[j])&&(i!=j)){
				flag=0;
				break;
			}else{ flag=1; }
		}
		if(flag){
			printf("First character not repeated:  %c\n",g[i]);
			return 0;
		}
	}
	printf("All character repeated\n");

	clock_gettime(CLOCK_REALTIME,&t2);
	printf("Exec time: %d ns",(t2.tv_nsec-t1.tv_nsec));
	return 0;
}
