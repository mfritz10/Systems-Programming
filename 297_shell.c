#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>

int original_stdout = -1;
static int MAXLINE = 100;

void sigintHandler(int sig) {
    printf("\ncaught sigint\n"); 
}

void sigtstpHandler(int sig) {
    printf("\ncaught sigstp\n");
}

void parseline2(char *buf, char **array, int arg){

    char *delim;
    int argc;

    buf[strlen(buf) -1] = ' ';
    
    while (*buf && (*buf == ' '))
        buf++;

    argc = arg;
    while ((delim = strchr(buf, ' '))) {
        array[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' '))
            buf++;
    }

    array[argc] = NULL;
}

void parseline(char *buf, char **array){
    parseline2(buf, array, 0);
}

void readInput(char **array, int argc) {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char command[100];
    if (fgets(command, sizeof(command), file) == NULL) {
        perror("Error reading from file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char *newline = strchr(command, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    fclose(file);
    strcat(command, " ");
    
    parseline2(command, array, argc);
}

void resetRedirection() {
    if (dup2(original_stdout, STDOUT_FILENO) == -1) {
        perror("Error in dup2 (reset)");
        exit(EXIT_FAILURE);
    }

    close(original_stdout);
    original_stdout = -1;
}

void redirectToFile(){
    original_stdout = dup(STDOUT_FILENO);
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    close(fd);
    return;
}

int buildin_command(char **argv) {
    if (!strcmp(argv[0], "exit"))
        exit(0);

    return 0;
}

void eval(char *cmdline){
    int MAXARGS = 20;
    char *array[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid, pid2;

    strcpy(buf, cmdline);
    
    parseline(buf, array);
    
    if (array[0] == NULL)
        return;

    char path[50] = "/usr/bin/";
    char *envp[] = {NULL};

    strcat(path, array[0]);

    char targetChar = '>';
    char targetChar2 = '<';
    char targetChar3 = ';';

    char *dummy[MAXARGS];
    for (int i = 0; i < MAXARGS; i++) {
        dummy[i] = NULL;
    }
    dummy[0] = "dummy";
    
    
    int found = 0;
    int found3 = 0;
    int index = 0;
    int i = 0;
    
    while (array[i] != NULL){
        if (strchr(array[i], targetChar) != NULL) {
            found = 1;
            array[i] = NULL;
        } else if (strchr(array[i], targetChar2) != NULL){
            readInput(array, i);
        } else if (strchr(array[i], targetChar3) != NULL){
            found3 = 1;
            index = i;
            
        }
        i++;
    }

    if (found) {
        redirectToFile();
    }

    if (found3) {
        int j = -1;
        

        while(array[index] != NULL) {
            if (j > -1){
                dummy[j] = array[index];
            }
            array[index] = NULL;
            index++;
            j++;
        }

        


    }


    if (!buildin_command(array)) {
        int status;
        int status2;
        if ((pid = fork()) == 0) {
            
            if (execve(path, array, envp) < 0) {
                printf("%s: Command not found. \n", array[0]);
                exit(0);
            }

        } else {
            if (original_stdout != -1) {
                resetRedirection();
            }
                
            
            if (waitpid(pid, &status, 0) < 0)
                printf("waitfg: waitpid error");
            }
        
        if (dummy[0] != "dummy"){
                
                char path2[50] = "/usr/bin/";
                strcat(path2, dummy[0]);
                if ((pid2 = fork()) == 0) {
                    if (execve(path2, dummy, envp) < 0) {
                        printf("%s: Command not found. \n", array[0]);
                        exit(0);
                    }
                } else {
                    if (original_stdout != -1) {
                        resetRedirection();
                    }
                    
                    if (waitpid(pid2, &status2, 0) < 0)
                        printf("waitfg: waitpid error");


                    printf("PID_1: %d\n", pid);
                    printf("Status_2: %d\n", status);
                    printf("PID_2: %d\n", pid2);
                    printf("Status_2: %d\n", status2);
                    }
                    

            } else {
            if (original_stdout != -1) {
                resetRedirection();
            }
                
            //int status;
            //if (waitpid(pid, &status, 0) < 0)
             //   printf("waitfg: waitpid error");


            printf("PID: %d\n", pid);
            printf("Status: %d\n", status);
            

            if (WIFEXITED(status)) {
                printf("Child process exited with status: %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Child process terminated by signal: %d\n", WTERMSIG(status));
            } else {
                printf("Child process terminated abnormally\n");
            }
        }
    }
        
    return;
    
    }
    

void main() {
    //int argc, char** argv
    char cmdline[MAXLINE];
    //char *filename = argv[0];

    signal(SIGINT, sigintHandler);
    signal(SIGTSTP, sigtstpHandler);

    while (1){
        
        printf("csci297 > ");
        fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)) 
            exit(0);

        eval(cmdline);


    }
}

