#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_ADDR 1000	/* Number of addresses in file */

/* Globals */
unsigned long addrs[NUM_ADDR];
FILE *addr_file;
FILE *disk_file;

/* Prototypes */
void read_addr_file(FILE *file, unsigned long *target);

int main(int argc, char *argv[]){
	unsigned short i;

	if(argc != 2){
		printf("Error: executable needs at least one input file\n");
		return 1;
	}
	
	addr_file = fopen(argv[1], "r");
	disk_file = fopen("./BACKING_STORE.bin","rw");

	read_addr_file(addr_file,addrs);
	
	for(i=0;i<NUM_ADDR;i++){
		printf("Address %d: %ld\n",i,addrs[i]);
	}

	fclose(addr_file);
	fclose(disk_file);

	return 0;

}

void read_addr_file(FILE *file,unsigned long *target){
	unsigned long k = 0;
	while(fscanf(file,"%ld",&target[k++]) != EOF);
}
