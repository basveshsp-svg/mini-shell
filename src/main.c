#include <stdio.h>
#include<string.h>   /* because we're using strtok() */

char command[1024];
char *command_ptr = command;
char *start = command;

int main() {

 while(1){
    printf("mini-shell> ");


    fflush(stdout);       /*To flush out output immediately after printf 
                           so characters are not stored in buffer 
                           before it's output*/

    command_ptr = fgets(command, sizeof(command), stdin);
    if(command_ptr==NULL){break;}
    char *mov_ptr = strtok(command, " ");   /*Notice first call of strtok
                                              takes arguments(command,
                                              delimiter) and from next 
                                              call it's (NULL, remaining delimiter)*/

	while(mov_ptr!= NULL){

          printf("\nToken: %s",mov_ptr);
          mov_ptr = strtok(NULL," ");

        }

  } 
   return 0;
}
