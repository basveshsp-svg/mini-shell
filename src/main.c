#include<signal.h>
#include<errno.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>   /* because we're using strtok() */

char command[1024];
char *command_ptr = command;
char *argv[100];
int token_number; 
pid_t fork_val;
char **ptr = argv;

int main() {

 while(1){

    printf("\nmini-shell> ");


    fflush(stdout);                                                             /* fflush() to flush out output immediately after printf 
                                                                                so characters are not stored in buffer 
                                                                                before it's output*/

    command_ptr = fgets(command, sizeof(command), stdin);


    if(command_ptr==NULL){break;}

    token_number = 0;

    char *mov_ptr = strtok(command, " \t\n");                                   /*Notice first call of strtok()
                                                                                takes arguments(command,
                                                                                delimiter) and from next
                                                                                call it's (NULL, remaining delimiter)*/
    while(mov_ptr!= NULL){

        argv[token_number] = mov_ptr;
        token_number++;
        mov_ptr = strtok(NULL," \t\n");

        } argv[token_number] = NULL;                                     /*It is very essential to end argv with NULL because so execvp will know when
                                                                         arguments have ended */
     if (argv[0] == NULL) {  continue; }
     if (!(strcmp(argv[0],"exit"))) { break; }
     if (!(strcmp(argv[0], "cd" ))) { if (argv[1]) { chdir(*(argv+1)); continue;  } else { chdir(getenv("HOME")); continue;} }


     fork_val = fork();

     if (fork_val == 0){

       int exec_val = execvp(argv[0],argv);
       if(exec_val == -1) { perror("execvp failed"); _exit(1); }        /* _exit(1) is called instead of exit(1) coz _exit(1)
                                                                            avoids flushing parent buffers (may print duplicate outputs) */

     }
        else if(fork_val==-1) { printf("\n Error"); continue; }
        else { waitpid(fork_val,NULL,0);}                                 /*waitpid(fork_val,NULL,0) fork_val tells which child to wait for, NULL 
                                                                            is exit status of child. so NULL in argument means idc how child exited, wait.
									    the third argument 0 tells which kind of wait*/

  }
   return 0;
}
