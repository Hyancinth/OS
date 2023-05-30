#include <stdio.h>
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdlib.h> /*For file descriptors*/

/*Define the needed macros*/
#define INT_SIZE 4 // Size of integer in bytes
#define INT_COUNT 10 
#define MEMORY_SIZE INT_COUNT * INT_SIZE

// Define character pointer to store the starting address of the memory mapped file

//Define integer array to store numbers from the memory mapped file.
//int intArray[MEMORY_SIZE]; 
int intArray[INT_COUNT];
signed char *mmapfptr; //pointer to section in RAM where we are copying the HDD memory (numbers.bin in this case) to


int main(int argc, const char *argv[])
{   
	int sum = 0;
    //Use the open() system call to open and read the numbers.bin file.
    //we are treating numbers.bin as the slow memory we want to map to RAM
    int mmapfile_fd = open("numbers.bin", O_RDONLY);

    //Use the mmap() system call to memory map numbers.bin file
  	mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);

    //Use a for loop to retrieve the contents of the memory mapped file and store it in the integer array using memcpy() function.
	for(int i = 0; i < INT_COUNT; i++){
		memcpy(intArray + i, mmapfptr + 4*i, INT_SIZE);
	}    

    //Sum up all the numbers
	for(int i = 0; i < INT_COUNT; i++){
		sum += intArray[i];
	}
    //Print Sum
	printf("Sum of numbers = %d\n", sum);
    //unmap the memory file
	munmap(mmapfptr, MEMORY_SIZE); 
    return 0;
}
