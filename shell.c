#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 80

//Max number of commands in history array
#define MAXHISTORY 5

char history[MAXHISTORY][MAXLINE];  //History array
int numHistCmd = 0; //Total number of commands in history

//Function prototypes
void execCmd(char ** args, int concurrent);
void updateHistory(char* command);
void printHistory();

//Main function
int main(void){
    char *args[MAXLINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */

    //While the flag is set to 1
    while(should_run){
        //Array to store command from command line
        char command[MAXLINE];

        int concurrent = 0; //Flag to determine if parent and child should be run concurrently
        int numArgs = 0; //Number of arguments in command

        printf("osh>");
        fflush(stdout);

        //Read from command line and store in command
        fgets(command, MAXLINE, stdin);

        //Remove newline character from command
        command[strcspn(command, "\n")] = 0;

        //If the command is empty, loop again
        if(strlen(command) == 0){
            continue;
        }

        //If the command is exit
        if(strcmp(command, "exit") == 0){
            //set the flag for the while loop to 0 and break out of the loop
            should_run = 0;
            break;
        }

        //If the command is history
        if(strcmp(command, "history") == 0){
            //run the print history function
            printHistory();
            continue;
        }

        //If the command is !!
        if(strcmp(command, "!!") == 0){
            //If history array is empty
            if(numHistCmd == 0){
                printf("No commands in history \n");
                continue;
            }
            //Copy the last command in history into the command array
            strcpy(command, history[4]);
        }

        //Tokenize command with delimiter " "
        char *cmdTok = strtok(command, " ");
        //Loop through command and store each token in args array
        while(cmdTok != NULL && numArgs < MAXLINE/2 + 1){
            //If the token is &, set the concurrent flag to 1
            if(strcmp(cmdTok, "&") == 0){
                concurrent = 1;
            }
            //Store token in args array
            else{
                args[numArgs] = cmdTok;
                numArgs++;
            }
            //Get next token
            cmdTok = strtok(NULL, " ");
        }
        //Null terminate args array
        args[numArgs] = NULL;

        //Execute command
        execCmd(args, concurrent);

        //Update history
        updateHistory(command);
    }

    return 0;
}

//Function that executes the command
void execCmd(char** args, int concurrent){
    //Fork, and get pid of child process
    pid_t pid = fork();

    //Child process
    if(pid == 0){
        //If the command is invalid
        if(execvp(args[0], args) == -1){
            printf("Unable to execute command \n");
        }
    }
    //Parent process
    else{
        //If the command is not concurrent, the parent waits for the child process to finish
        if(concurrent == 0){
            wait(NULL);
        }
    }
}

//Function that updates history array
void updateHistory(char* command){
    //If the total number of commands in history is less than the max number of commands
    //Add it directly into the array (newest command is stored at the end of the array)
    if(numHistCmd < MAXHISTORY){
        strcpy(history[numHistCmd], command);
        numHistCmd++;
    }
    //If the total number of commands in history is greater than the max number of commands
    else{
        //Shift all commands in history array up by one
        for(int i = 0; i < MAXHISTORY - 1; i++){
            strcpy(history[i], history[i+1]);
        }
        //Add new command to the end of the array and increment number of commands in history
        strcpy(history[MAXHISTORY - 1], command);
        numHistCmd++;
    } 
}

//Function that prints the history array
void printHistory(){
    //If the number of total commands in history is less than the max number of commands, print all commands in history array
    if(numHistCmd < MAXHISTORY){
        for(int i = 0; i < numHistCmd; i++){
            //The newest command is stored at the end of the array, so we print the array backwards to match example output
            printf("%d %s \n", (numHistCmd - i), history[numHistCmd - i -1]);
        }
    }
    //If the number of total commands history is greated than the max number of commands
    else{
        for(int i = 0; i < MAXHISTORY; i++){
            //MAXHISTORY is constant at 5, so when numHistCmd is greater than MAXHISTORY
            //We use MAXHISTORY - i - 1 to get the correct index of the history array 
            //We can't use numHistCmd - i - 1 because numHistCmd will be greater than the bounds of the history array
            printf("%d %s \n", (numHistCmd - i), history[MAXHISTORY - i -1]);
        }
    }
}