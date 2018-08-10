#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sort(int list[],int size){
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

//Dynamic array
typedef struct {
	unsigned int *array;
	unsigned short size;
	unsigned int used_entries;
} D_Array;

void init_D_Array(D_Array *arr,unsigned short size){
	arr->array=(unsigned int *)malloc(size*sizeof(unsigned int));	//Allocate memory space for array specified
	arr->used_entries = 0;
	arr->size=size;
}

void intnsert_D_Array(D_Array *arr,unsigned int item){
	if(arr->used_entries == arr->size){		//If dynamic array full
		arr->size += 1;						//Increase size when needed
		arr->array = (unsigned int *)realloc(arr->array,arr->size * sizeof(unsigned int));	//Reallocate memory space for array with new size
	}
	arr->array[arr->used_entries++] = item;		//Insert item after size increase condition
}

//

unsigned int median(unsigned int *arr, unsigned int size){
	unsigned int m;
	if(size%2==0){
		m = (arr[size/2]+arr[(size/2)+-1])/2;
	}else{
		m = arr[size/2];
	}
	return m;
}

int main(void){

	struct timespec t1,t2;
	clock_gettime(CLOCK_REALTIME,&t1);

	D_Array numbers;
	init_D_Array(&numbers,0);		//Function ask for pointer as parameter so pointer given, initial size of 0 element (insert increases size as needed)

	unsigned char flag=0;
	unsigned int input_number;		//Buffer for input numbers
	char option;					//Option selection buffer

	do{
		printf("Insert element or calculate median? i | m\nx to exit program\n> ");
		option=getchar();
		if(option=='i'){
			printf("Insert positive integer: ");
			scanf("%d%*c",&input_number);
			if(input_number<0){
				printf("Only positive integers");
			}else{insert_D_Array(&numbers,input_number);}
		}else if(option=='m'){
			if(numbers.size!=0){
				sort(numbers.array,numbers.size);
				printf("Median: %d\n", median(numbers.array,numbers.size));
			}else{printf("No elements in array\n");}
		}else if(option=='x'){
			printf("Program terminated\n");
			flag=1;
		}else{
			printf("Please choose a valid option\n");
		}
	}while(!flag);

	clock_gettime(CLOCK_REALTIME,&t2);
	printf("Exec time: %d ns",(t2.tv_nsec-t1.tv_nsec));
	return 0;
}
