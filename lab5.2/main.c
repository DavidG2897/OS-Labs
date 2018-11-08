#include <stdio.h>
#include <stdlib.h>

#define NUM_ADDR 1000				/* Number of addresses in file */
#define TLB_SIZE 16				/* TLB entries */
#define PAGE_TABLE_SIZE 256			/* Number of pages */
#define PAGE_SIZE 256				/* Page size in bytes */
#define FRAME_SIZE 256				/* Frame size in bytes */
#define NUM_FRAMES 256				/* Number of frames */
#define PHY_MEM_SIZE FRAME_SIZE*NUM_FRAMES	/* Physical memory size in bytes */
#define READ_FILE(a,b) k = 0; \
	while(fscanf(a,"%ld",&b[k++].dword) != EOF);


/* Datatypes */
typedef union{
	unsigned char bytes[4];	/* Byte 0: offset, Byte 1: page number, last two bytes are not used (maybe data?) */
	unsigned short word[2];
	unsigned long dword;
} Address;

/* Globals */
Address addresses[NUM_ADDR];
FILE *addr_file;
FILE *disk_file;

/* Prototypes */
//No prototypes at the moment

int main(int argc, char *argv[]){

	unsigned short i,k;	/* Only use k to read files with READ_FILE macro! */
	unsigned char str[80];

	if(argc != 2){
		printf("Error: executable needs at least one input file\n");
		return 1;
	}
	
	addr_file = fopen(argv[1], "r");
	disk_file = fopen("./BACKING_STORE.bin","rb");
	
	if(disk_file == NULL){
		printf("Error reading disk file");
		return 1;
	}

	READ_FILE(addr_file,addresses);

	for(i=0;i<NUM_ADDR;i++){
		printf("Address %ld: %ld (0x0000%X)\n\t",i,addresses[i].dword,addresses[i].dword);
		for(k=1;k!=65535;k--){
			printf("%s: %X ",(k == 0) ? "Offset" : "Page",addresses[i].bytes[k]);
		}
		printf("\n");
	}


	fseek(disk_file,10*4,SEEK_SET);
	fread(str,sizeof(str),1,disk_file);
	for(i=0;i<80;i++){
		printf("%2X",str[i]);
	}

	printf("\n");

	fclose(addr_file);
	fclose(disk_file);

	return 0;

}

