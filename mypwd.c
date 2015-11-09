#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define DIR_SIZE 1024

void print_help();

int main(int argc, char** argv) {
    if(argc==2 && strcmp(argv[1], "--help") == 0){
        print_help();
    }else {
        char cwd[DIR_SIZE];

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            fprintf(stderr, "mypwd : get current directory error\n");
        }
    }
}

void print_help() {
    printf("NAME\n");
    printf("\t mypwd - return working directory name \n");
    printf("SYNOPSIS\n");
    printf("\t mypwd \n");
}