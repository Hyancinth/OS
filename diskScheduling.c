/*
    Implementation of various disk scheduling algorithms
*/
#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//define macros
#define CYLINDERS 300
#define REQUESTS 20 

//request arrays
int requests[REQUESTS];
int sortedRequests[REQUESTS];

//function prototypes
void sortArr(int *arr);
void FCFS(int initialPosition, int *requests);
void SSTF(int initialPosition, int *requests);
void CSCAN(int initialPosition, char *direction, int *sortedRequests);
void SCAN(int initialPostion, char *direction, int *sortedRequests);
void LOOK(int initialPosition, char *direction, int *sortedRequests);
void CLOOK(int initialPosition, char *direction, int *sortedRequests);

void removeElement(int*arr, int index, int size);

int main(int argc, char *argv[]){

    //check for correct number of arguments
    if(argc != 3){
        printf("Error: Invalid number of arguments");
    }

    //check for valid direction
    if(strcmp(argv[2], "LEFT") != 0 && strcmp(argv[2], "RIGHT") != 0) {
        printf("Direction must be either LEFT or RIGHT\n");
    }

    //read command line arguments
    int initialPosition = atoi(argv[1]);
    char *direction = argv[2];

    //open the binary file and store contesnts in requests array
    int readRequests = open("request.bin", O_RDONLY);
    read(readRequests, requests, sizeof(requests));
    //close the file
    close(readRequests);

    //copy requests array to sortedRequests array
    for(int i = 0; i < REQUESTS; i++){
        sortedRequests[i] = requests[i];   
    }

    //sort the sortedRequest array
    sortArr(sortedRequests);

    printf("Total requests = %d\n", REQUESTS);
    printf("Initial head position = %d\n", initialPosition);
    printf("Direction of Head = %s\n", direction);

    //call functions 
    FCFS(initialPosition, requests);
    SSTF(initialPosition, requests);
    SCAN(initialPosition, direction, sortedRequests);
    CSCAN(initialPosition, direction, sortedRequests);
    LOOK(initialPosition, direction, sortedRequests);
    CLOOK(initialPosition, direction, sortedRequests);

    return 0;
}

void sortArr(int *arr){
    int temp = 0;
    for(int i = 0; i < REQUESTS-1; i++){
        for(int j = 0; j < REQUESTS-i-1; j++){
            if(arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

//First Come First Serve
//note in this algorithm direction doesn't matter
void FCFS(int initialPosition, int *requests){
    int totalHeadMovements = 0;
    int currentHeadPosition;

    printf("\nFCFS DISK SCHEDULING ALGORITHM\n\n");
    //go through request array
    for(int i = 0; i < REQUESTS; i++){
        //set current head position to the current request
        currentHeadPosition = requests[i];
        //calculate total head movements
        totalHeadMovements += abs(initialPosition - currentHeadPosition);
        //set initial position to current head position for next iteration 
        initialPosition = currentHeadPosition;
        printf("%d, ", currentHeadPosition);
    }
    printf("\n\nFCFS - Total head movements = %d\n", totalHeadMovements);
}

//Shortest Seek Time First
//note in this algorithm direction doesn't matter
void SSTF(int initialPosition, int *requests){
    int totalHeadMovements = 0;
    int currentHeadPosition = initialPosition;
    
    int totalRequests = REQUESTS;
    int minDiff; //minimum difference between current head position and request
    int temp = 0;
    int index = 0;


    int copyRequests [REQUESTS]; //copy of requests array
    for(int i = 0; i < REQUESTS; i++){
        copyRequests[i] = requests[i];   
    }
    int sizeOfArr = sizeof(copyRequests) / sizeof(int);

    printf("\nSSTF DISK SCHEDULING ALGORITHM\n\n");
    //loop for the total number of requests
    while(totalRequests > 0){
        minDiff = CYLINDERS; //set minDiff to max value
        //Go through the requests and find the request with the shortest seek time
        for(int i = 0; i < sizeOfArr; i++){
            temp = abs(currentHeadPosition - copyRequests[i]);
            if(temp < minDiff){
                minDiff = temp;
                index = i; //index of the request with the shortest seek time
            }
        }

        //set current head position to the request with the shortest seek time
        currentHeadPosition = copyRequests[index];
        //calculate total head movements
        totalHeadMovements += abs(initialPosition - currentHeadPosition);
        //set initial position to current head position for next iteration
        initialPosition = currentHeadPosition;

        printf("%d, ", currentHeadPosition);

        //remove the request with the shortest seek time from the array
        removeElement(copyRequests, index, sizeOfArr);
        sizeOfArr -= 1;
        totalRequests -= 1;
    }
    printf("\n\nFCFS - Total head movements = %d\n", totalHeadMovements);
}

//function that removes elements from an array (Used for SSTF)
void removeElement(int *arr, int index, int size) {
    for (int i = index; i < size - 1; i++) {
        arr[i] = arr[i+1];
    }
}

//SCAN 
void SCAN(int initialPosition, char *direction, int *sortedRequests){
    int totalHeadMovements = 0;
    int currentHeadPosition;
    int totalRequests = REQUESTS;

    int index = 0;

    //find the index of the initial position
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] == initialPosition){
            index = i;
        }
    }

    //indexes used to travel through the array in the second loop 
    int secondLeftIndex = index + 1; //index to the right of the initial position
    int secondRightIndex = index - 1; //index to the left of the initial position
    int copyInitIndex = index;


    printf("\nSCAN DISK SCHEDULING ALGORITHM\n\n");

    //if direction is LEFT
    if(strcmp(direction, "LEFT") == 0){
        //loop for the total number of requests
        while(totalRequests > 0){
            //First looop from the initial position to the left
            if(index >= 0){
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == 0){
                    //calculate total head movements
                    totalHeadMovements += abs(sortedRequests[copyInitIndex] - 0); 
                }
                index -= 1;
            }
            //Second loop from the right side of the initial position (going left)
            else{
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[secondLeftIndex];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(secondLeftIndex ==  REQUESTS - 1){
                    //calculate total head movements
                    totalHeadMovements += abs(0 - sortedRequests[REQUESTS - 1]);
                }
                secondLeftIndex += 1;
            }
            totalRequests -= 1;
        }
        printf("\n\nSCAN - Total head movements = %d\n", totalHeadMovements);
    }
    //if direction is RIGHT
    else if(strcmp(direction, "RIGHT") == 0){
        //loop for the total number of requests
        while(totalRequests > 0){
            //First loop from the initial position to the right
            if(index <= REQUESTS - 1){
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == REQUESTS - 1){
                    //calculate total head movements
                    totalHeadMovements += abs(sortedRequests[copyInitIndex] - (CYLINDERS - 1));
                }
                index += 1;
            }
            //Second loop from the left side of the initial position (going right)
            else{
                currentHeadPosition = sortedRequests[secondRightIndex];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(secondRightIndex == 0){
                    totalHeadMovements += abs((CYLINDERS - 1) - sortedRequests[0]);
                }
                secondRightIndex -= 1;
            }
            totalRequests -= 1;
        }
        printf("\n\nSCAN - Total head movements = %d\n", totalHeadMovements);
    }
}

void CSCAN(int initialPosition, char *direction, int *sortedRequests){
    int totalHeadMovements = 0;
    int currentHeadPosition;  

    //indexes used to travel through the array 
    int index = 0; //used for first half of the algorithm: to travel from initial position till the end of the array
    int secondLeftIndex = REQUESTS-1; //used for second half of the algorithm: to travel from end  of array to initial position - 1
    int secondRightIndex = 0;

    int totalRequests = REQUESTS;
    int initPos = initialPosition;

    //find the index of the initial position
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] == initialPosition){
            index = i;
        }
    }
    //copy index to remember the index of the initial position
    int copyInitIndex = index;

    printf("\nCSCAN DISK SCHEDULING ALGORITHM\n\n");
    //if direction is LEFT
    if(strcmp(direction, "LEFT") == 0){\
        //loop for the total number of requests
        while(totalRequests > 0){
            //First loop from the initial position to the left
            if(index >=0){
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == 0){
                    totalHeadMovements += abs(sortedRequests[copyInitIndex] - sortedRequests[0]);
                    //circular movement
                    totalHeadMovements += abs(sortedRequests[0] - 0);
                    totalHeadMovements += abs(0 - 299);
                }
                index-=1;
            }
            //Second loop the end of the array to the initial position - 1
            else{
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[secondLeftIndex];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(secondLeftIndex == copyInitIndex+1){
                    totalHeadMovements += abs(299 - sortedRequests[copyInitIndex+1]);
                }
                secondLeftIndex -=1;
            }
            totalRequests -= 1;
        }
        printf("\n\nCSCAN - Total head movements = %d\n", totalHeadMovements);
        return;
    }
    //if direction is RIGHT
    else if(strcmp(direction, "RIGHT") == 0){
        //loop for the total number of requests
        while(totalRequests > 0){
            //First loop from the initial position to the right
            if(index <= REQUESTS - 1){
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == REQUESTS-1){
                    totalHeadMovements += abs(initialPosition - sortedRequests[REQUESTS-1]);
                    //circular movement
                    totalHeadMovements += abs(sortedRequests[REQUESTS - 1] - 299);
                    totalHeadMovements += abs(299 - 0);
                }
                index += 1;
            }
            //Second loop from the beginning of the array to the initial position - 1
            else{
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[secondRightIndex];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(secondRightIndex == copyInitIndex-1){
                    totalHeadMovements += abs(0 - sortedRequests[copyInitIndex-1]);
                }
                secondRightIndex += 1;
            }
            totalRequests -= 1;
        }
        printf("\n\nCSCAN - Total head movements = %d\n", totalHeadMovements);
        return;
    }
}

//LOOK 
void LOOK(int initialPosition, char *direction, int *sortedRequests){
    int totalHeadMovements = 0;
    int currentHeadPosition;
    int totalRequests = REQUESTS;

    int index = 0;
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] == initialPosition){
            index = i;
        }
    }

    //indexes used to travel through the array in the second loop
    int secondLeftIndex = index + 1;
    int secondRightIndex = index - 1;
    int copyInitIndex = index;

    printf("\nLOOK DISK SCHEDULING ALGORITHM\n\n");
    //if direction is LEFT
    if(strcmp(direction, "LEFT") == 0){
        //loop for the total number of requests
        while(totalRequests > 0){
            //First loop from the initial position to the left
            if(index >= 0){
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == 0){
                    totalHeadMovements += abs(sortedRequests[copyInitIndex] - sortedRequests[0]);
                }
                index -= 1;
            }
            //second loop from the right of the initial position (going left)
            else{
                currentHeadPosition = sortedRequests[secondLeftIndex];
                printf("%d, ", currentHeadPosition);
                if(secondLeftIndex == REQUESTS - 1){
                    totalHeadMovements += abs(sortedRequests[0] - sortedRequests[REQUESTS - 1]);
                }
                secondLeftIndex += 1;
            }
            totalRequests -= 1;
        }
        printf("\n\nLOOK - Total head movements = %d\n", totalHeadMovements);
    }
    //if direction is RIGHT
    else if(strcmp(direction, "RIGHT") == 0){
        //loop for the total number of requests
        while(totalRequests > 0){
            //First loop from the initial position to the right
            if(index <= REQUESTS - 1){
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == REQUESTS - 1){
                    totalHeadMovements += abs(sortedRequests[copyInitIndex] - sortedRequests[REQUESTS - 1]);
                }
                index += 1;
            }
            //second loop from the left of the initial position (going right)
            else{
                currentHeadPosition = sortedRequests[secondRightIndex];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(secondRightIndex == 0){
                    totalHeadMovements += abs(sortedRequests[REQUESTS - 1] - sortedRequests[0]);
                }
                secondRightIndex -= 1;
            }
            totalRequests -= 1;
        }
        printf("\n\nLOOK - Total head movements = %d\n", totalHeadMovements);
    }
}

//CLOOK
void CLOOK(int initialPosition, char *direction, int *sortedRequests){
    int totalHeadMovements = 0;
    int currentHeadPosition;
    int totalRequests = REQUESTS;

    int index = 0;
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] == initialPosition){
            index = i;
        }
    }
    
    int secondLeftIndex = REQUESTS - 1;
    int secondRightIndex = 0;
    int copyInitIndex = index;

    printf("\nCLOOK DISK SCHEDULING ALGORITHM\n\n");
    
    while(totalRequests > 0){
        //if direction is LEFT  
        if(strcmp(direction, "LEFT") == 0){
            if(index >= 0){
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == 0){
                    //circular movement
                    totalHeadMovements += abs(sortedRequests[copyInitIndex] - sortedRequests[0]);
                    totalHeadMovements += abs(sortedRequests[0] - sortedRequests[REQUESTS - 1]);
                }
                index -= 1;
            }
            //second loop from the end of the arrah (going left)
            else{
                currentHeadPosition = sortedRequests[secondLeftIndex];
                printf("%d, ", currentHeadPosition);
                if(secondLeftIndex == copyInitIndex + 1){
                    totalHeadMovements += abs(sortedRequests[REQUESTS - 1] - sortedRequests[copyInitIndex + 1]);
                }
                secondLeftIndex -= 1;
            }
        }
        //if direction is RIGHT
        else if(strcmp(direction, "RIGHT") == 0){
            if(index <= REQUESTS - 1){
                //set current head position to the request at the index
                currentHeadPosition = sortedRequests[index];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(index == REQUESTS - 1){
                    //circular movement
                    totalHeadMovements += abs(sortedRequests[copyInitIndex] - sortedRequests[REQUESTS - 1]);
                    totalHeadMovements += abs(sortedRequests[REQUESTS - 1] - sortedRequests[0]);
                }
                index += 1;
            }
            //second loop from the beginning of the array (going right)
            else{
                currentHeadPosition = sortedRequests[secondRightIndex];
                printf("%d, ", currentHeadPosition);
                //calculations done in one step
                if(secondRightIndex == copyInitIndex - 1){
                    totalHeadMovements += abs(sortedRequests[0] - sortedRequests[copyInitIndex - 1]);
                }
                secondRightIndex += 1;
            }
        }
        totalRequests -= 1;
    }
    printf("\n\nCLOOK - Total head movements = %d\n", totalHeadMovements);
}
