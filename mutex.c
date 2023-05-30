#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMThreads 3
#define true 1

//Initialize mutex lock
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

/*Declare the thread functions*/
void *deposit(void *param);
void *withdraw(void *param);

/*Declare and initialize shared variable*/
int amount = 0;


int main(int argc, char *argv[])
{
    /*Error handling code for the input*/
    if(argc != 3){
        printf("Invalid number of arguments. Please enter 2 arguments");
        return 0;
    }
    
    /*Get deposit and withdraw amounts */
    int depositAmount = atoi(argv[1]);
    int withdrawAmount = atoi(argv[2]);

    /*Declare and initialize thread attribute*/
    pthread_t tid[NUMThreads*2];
    
    /*Declare and Create Deposit and Withdraw threads*/

    //Deposit threads have tid's starting from 0 to 2
    for(int i = 0; i < NUMThreads; i++)
    {
        if(pthread_create(&tid[i], NULL, deposit, (void*) &depositAmount) != 0){
            printf("Deposit thread creation failed");
        }
    }

    //Withdraw threads have tid's starting from 3 to 5
    for(int i = 3; i < NUMThreads*2; i++){
        if(pthread_create(&tid[i], NULL, withdraw, (void*) &withdrawAmount) != 0){
            printf("Withdraw thread creation failed");
        }
    }
    
    /*Join Deposit threads*/
    for(int i = 0; i < NUMThreads; i++){
        pthread_join(tid[i], NULL);
    }

    /*Join withdraw threads*/
    for(int i = 3; i < NUMThreads*2; i++){
        pthread_join(tid[i], NULL);
    }
    
    /*Print Final Amount*/
    printf("Final amount = %d \n", amount);
    
	return 0;
}

void *deposit(void *param) {
	while (true) { /* deposit 100 */
        //Convert from void pointer to int
        int deposit = *(int *) param;

        //Acquire lock
        pthread_mutex_lock(&mutex_lock);
        amount += deposit;

        printf("Amount after deposit = %d \n", amount);

        //Release lock
        pthread_mutex_unlock(&mutex_lock);

	    pthread_exit(0);
	}
	
}

void *withdraw(void *param) {

	while (true) { /* withdraw 50 */
        //Convert from void pointer to int
        int withdraw = *(int *) param;

        //Acquire lock
        pthread_mutex_lock(&mutex_lock);
        amount -= withdraw;

        printf("Amount after withdraw = %d \n", amount);

        //Release lock
        pthread_mutex_unlock(&mutex_lock);

        pthread_exit(0);
	}
	
}

