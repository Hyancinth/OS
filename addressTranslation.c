/*
*
* Author: Prof. Neerja Mhaskar
* Course: Operating Systems Comp Sci/SFWRWENG 3SH3

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//Define the needed macros
#define BUFFER_SIZE 10

#define OFFSET_MASK 4095 //2^OFFSET_BITS - 1
#define PAGES 8
#define OFFSET_BITS 12 //(12, from the page size)
#define PAGE_SIZE 4096 //2^12 (4KB)

int main(int argc, char *argv[]) {
    //declare variables and declare and initialize the page table.
	int pageNumber;
	int pageOffset;
	int logicalAddress;
	int physicalAddress;

	int page_table[PAGES] = {6,4,3,7,0,1,2,5};
	/*Declare a file pointer fptr. fopen initializes an object of type file.
	 This is stored in fptr. The mode 'r' indicates that the file is for 
	read only purpose.*/
	
	FILE *fptr = fopen("labaddr.txt", "r");

	/*Read characters from the file at a time.
	These characters are stored in buff to read the logical addresses*/
	char buff [BUFFER_SIZE];
	    
    /*While you read the logical addresses using bitwise operators in C to compute the physical address for each logical address read and print it*/
	
	while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
		//printf("%s", buff);
		logicalAddress = atoi(buff);
		pageNumber = logicalAddress >> OFFSET_BITS;
		pageOffset = logicalAddress & OFFSET_MASK;
		//page frame is the correspending frame in the page table at the index of the page number
		physicalAddress = (page_table[pageNumber] << OFFSET_BITS) | pageOffset;
		printf("logical address: %d, Page Number: %d, Page Offset: %d, Physical Address:%d \n", logicalAddress, pageNumber, pageOffset, physicalAddress);
	}

	/*Close the file.*/
	fclose(fptr);


	return 0;
}
