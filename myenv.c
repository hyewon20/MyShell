#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;
void print_help();

int main(int argc, char** argv) {
    if(argc >= 2) {
        const char *env = getenv(argv[1]);
        if(strcmp(argv[1], "--help") == 0){
            print_help();
        }
        else if(env == NULL){
            fprintf(stderr, "%s: No such environment key.\n", argv[1]);
        }else{
            printf("%s\n", env);
        }
    }else{
        int i;
        for (i=0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]);
        }
    }
    return 1;
}

void print_help(){
    printf("NAME\n");
    printf("\t myenv - set environment and execute command, or print environment\n");
    printf("SYNOPSIS\n");
    printf("\t myenv [STRING]\n");
    printf("\t[STRING]: option, split apart the given string into multiple strings.\n");
}