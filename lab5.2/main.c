#include <stdio.h>
#include <stdlib.h>

#define NUM_ADDR 1000				/* Number of addresses in file */
#define TLB_SIZE 16				/* TLB entries */
#define PAGE_TABLE_SIZE 256			/* Number of pages */
#define PAGE_SIZE 256				/* Page size in bytes */
#define FRAME_SIZE 256				/* Frame size in bytes */
#define NUM_FRAMES 256				/* Number of frames */
#define READ_FILE(a,b) k = 0; \
	while(fscanf(a,"%ld",&b[k++].dword) != EOF);
#define READ_DISK_PAGE(a,b) 	fseek(disk_file,b*FRAME_SIZE,SEEK_SET); \
	fread(a[current_frame],sizeof(signed char),FRAME_SIZE,disk_file)
#define INVALID -1
#define UPDATE_TLB(a,b) tlb[tlb_index].page = a; \
	tlb[tlb_index].frame = b; \
	tlb_index++; \
	if(tlb_index == TLB_SIZE) tlb_index = 0
	/* FIFO */

/* Datatypes */
typedef union{
	unsigned char bytes[4];	/* Byte 0: offset, Byte 1: page number, last two bytes are not used (maybe data?) */
	unsigned short word[2];
	unsigned long dword;
} Address;

typedef struct{
	unsigned char page;
	unsigned char frame;
} Entry;

/* Globals */
Address addresses[NUM_ADDR];
Entry tlb[TLB_SIZE];
char page_table[PAGE_TABLE_SIZE];
char mem[NUM_FRAMES][FRAME_SIZE];
FILE *addr_file;
FILE *disk_file;

/* Prototypes */
void init(void);
void readAddresses(Address *input, unsigned long num_addr);

int main(int argc, char *argv[]){

	unsigned short k = 0;	/* Only use k to read files with READ_FILE macro! */

	init();

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

	readAddresses(addresses,NUM_ADDR);

	fclose(addr_file);
	fclose(disk_file);

	return 0;

}

void init(void){
	unsigned short i = 0;	

	for(i=0;i<TLB_SIZE;i++){
		tlb[i].page=INVALID;
	}

	for(i=0;i<PAGE_TABLE_SIZE;i++){
		page_table[i] = INVALID;
	}

}

void readAddresses(Address *input, unsigned long num_addr){

	unsigned char tlb_hit = 0, page = 0, offset = 0, current_frame = 0, tlb_index = 0;
	signed char value = 0;
	unsigned char *str;
	unsigned short i = 0, k = 0, n = 0;
	Address physical_addr;
	physical_addr.dword = 0;
	FILE *output = fopen("./output.txt","w");

	for(i=0;i<num_addr;i++){
		page = input[i].bytes[1];
		offset = input[i].bytes[0];
		physical_addr.bytes[0] = offset;
		for(n=0;n<TLB_SIZE;n++){
			if(tlb[n].page == page){
				physical_addr.bytes[1] = tlb[n].frame;
				tlb_hit = 1;
			}else{tlb_hit = 0; }
		}

		if(!tlb_hit){
			if(page_table[page] == INVALID){	//If page fault...
				page_table[page] = current_frame;
				READ_DISK_PAGE(mem,page);	//Read disk for missing page
				UPDATE_TLB(page,current_frame);
				physical_addr.bytes[1] = current_frame;
				value = mem[current_frame][offset];
				current_frame++;
			}else{
				physical_addr.bytes[1] = (unsigned char) page_table[page];
				value = mem[(unsigned char)page_table[page]][offset];
			}
		}
		fprintf(output,"Virtual: %ld\tPhysical: %ld\tValue: %ld\n",input[i].dword,physical_addr.dword,value);
	}
	fclose(output);
}

