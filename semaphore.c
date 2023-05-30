#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h> 


#define NUMDepositThreads 7
#define NUMWithdrawThreads 3
#define true 1
#define LIMIT 400

//Initialize mutex lock
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

//Declare semaphores
sem_t fullSem;
sem_t emptySem;

/*Declare the thread functions*/
void *deposit(void *param);
void *withdraw(void *param);

/*Declare and initialize shared variable*/
int amount = 0;
int firstDeposit = 0;

int main(int argc, char *argv[])
{
	/*Error handling code for the input*/
    if(argc != 2){
        printf("Invalid number of arguments. Please enter 1 argument");
        return 0;
    }

    //Get money 
    int money = atoi(argv[1]);

    
    /*Declare and initialize thread attribute*/
    pthread_t tid[NUMDepositThreads+NUMWithdrawThreads];
	
    /*Create Deposit and Withdraw threads*/

    //Deposit threads have tid's starting from 0 to 6
    for(int i = 0; i < NUMDepositThreads; i++)
    {
        if(pthread_create(&tid[i], NULL, deposit, (void*) &money) != 0){
            printf("Deposit thread creation failed");
        }
    }
	
    //Withdraw threads have tid's starting from 7 to 9
    for(int i = 7; i < NUMDepositThreads+NUMWithdrawThreads; i++){
        if(pthread_create(&tid[i], NULL, withdraw, (void*) &money) != 0){
            printf("Withdraw thread creation failed");
        }
    }

	
    //Amount starts at 0 so we want
	if(sem_init(&fullSem, 0, 0) != 0){
        printf("Deposit semaphore initialization failed");
    }

    //Amount starts at 0 so withdraw needs to wait until amount is greater than 0
    int max = LIMIT/100;
    if(sem_init(&emptySem, 0, max) != 0){
        printf("Widthdraw semaphore initialization failed");
    }

   
    /*Join Deposit threads*/
    for(int i = 0; i < NUMDepositThreads; i++){
        pthread_join(tid[i], NULL);
  
    }

    /*Join withdraw threads*/
	for(int i = 7; i < NUMDepositThreads+NUMWithdrawThreads; i++){
        pthread_join(tid[i], NULL);
    }

    /*Print Final Amount*/
    printf("Final amount = %d \n", amount);

    sem_destroy(&fullSem);
    sem_destroy(&emptySem);
    pthread_mutex_destroy(&mutex_lock);
	return 0;
}

void *deposit(void *param) {
	while (true) {      
        //get deposit amount
        int deposit = *(int*)param;
        sem_wait(&emptySem);
        pthread_mutex_lock(&mutex_lock);

        printf("Executing deposit function\n");
        amount += deposit;
        printf("Amount after deposit = %d \n", amount);

        pthread_mutex_unlock(&mutex_lock);

        sem_post(&fullSem);
		pthread_exit(0);
	}
	
}

void *withdraw(void *param) {
	while (true) {
        int withdraw = *(int*)param;

        //wait for the first deposit to be made
        sem_wait(&fullSem);

        pthread_mutex_lock(&mutex_lock);

        printf("Executing withdraw function\n");
        amount -= withdraw;
        printf("Amount after withdraw = %d \n", amount);

        pthread_mutex_unlock(&mutex_lock);


        sem_post(&emptySem);
		pthread_exit(0);
	}
	
}

