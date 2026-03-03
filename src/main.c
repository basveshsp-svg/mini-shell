#include <fcntl.h>
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
/*===============Tokenization=================*/

    while(mov_ptr!= NULL){

        argv[token_number] = mov_ptr;
        token_number++;
        mov_ptr = strtok(NULL," \t\n");

        } argv[token_number] = NULL;                                     /*It is very essential to end argv with NULL because so execvp will know when
                                                                         arguments have ended */

 /*=============Built-ins=================*/



     if (argv[0] == NULL) {  continue; }
     if (!(strcmp(argv[0],"exit"))) { break; }
     if (!(strcmp(argv[0], "cd" ))) { if (argv[1]) { chdir(*(argv+1)); continue;  } else { chdir(getenv("HOME")); continue;} }


/*=========== I/O Redirection ======================*/

    char *filename;
    int redirection_flag = 0;
    int empty_flag;

    for (int i=0; argv[i]!= NULL; i++){


	if(argv[i+1] && !(strcmp(argv[i],">")) && (strcmp(argv[i+1],"<"))) {

		redirection_flag = 1;
		filename = argv[i+1];
		argv[i] = NULL;
		argv[i+1] = NULL;
	}

        if(argv[i+1] && !(strcmp(argv[i],"<")) && (strcmp(argv[i+1],">"))) {

                redirection_flag = 2;
                filename = argv[i+1];
                argv[i] = NULL;
                argv[i+1] = NULL;

	}

	if(!(strcmp(argv[i],">")) && argv[i+1] == NULL) { printf("syntax error: missing filename after >"); empty_flag = 1; break; }
        if(!(strcmp(argv[i],"<")) && argv[i+1] == NULL) { printf("syntax error: missing filename after <"); empty_flag = 1; break; }


    } if (empty_flag) {continue;}



     fork_val = fork();
     int fd_val;

     if (fork_val == 0){

	if(redirection_flag == 1){
	       fd_val = open(filename , O_WRONLY);
       	       dup2(fd_val,1);
               close(fd_val);
	}


	if(redirection_flag == 2){
               fd_val = open(filename , O_RDONLY);
               dup2(fd_val,0);
               close(fd_val);
        }


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
