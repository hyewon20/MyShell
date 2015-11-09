#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define DIR_SIZE 1024
void print_help();

int main(int argc, char** argv) {
    int fd_src, fd_dest;    /* source file and dest file's file descriptor */
    char src_name[128], dest_name[128];
    int f_num;
    char buffer[6];

    if(argc<3){
        if(argc==2 && strcmp(argv[1], "--help") == 0){
            print_help();

            fprintf(stderr, "mymv : missing operand after 'mymv'\n");
            printf("mymv : Try 'mymv --help' for more information.\n");
            printf("usage : mymv [OPTION] [SOURCE] [DEST]\n");
        }
    }else {
        /* limit file name length not larger than 128 */
        if(strlen(argv[1]) > 128 || strlen(argv[2]) > 128) {
            printf("mymv : filename not over 128 byte\n");
            exit(1);
        }

        strcpy(src_name, argv[1]);
        strcpy(dest_name, argv[2]);

        fd_src = open(src_name, O_RDONLY);
        if(fd_src == -1) {
            fprintf(stderr, "mymv :%s : open file error\n", src_name);
            exit(1);
        }

        fd_dest = open(dest_name, O_CREAT| O_WRONLY | O_TRUNC, 0644);
        if(fd_dest == -1) {
            fprintf(stderr, "mymv :%s : copy file open errorr\n", dest_name);
            exit(1);
        }

        /* copy from [SOURCE] to [DEST] 4byte, when copy finished f_num returns 0 then while loop terminated */
        while((f_num = read(fd_src, buffer, 4)) > 0) {
            if(write(fd_dest, buffer, f_num) != f_num)
                fprintf(stderr, "mymv :%s :  file write errorr\n", dest_name);
        }

        if(f_num == -1) {
            fprintf(stderr, "mymv :%s :  file read errorr\n", src_name);
        }else

        remove(src_name);
        close(fd_src);
        close(fd_dest);
    }

    return 0;
}

void print_help() {
    printf("NAME\n");
    printf("\t mymv - move files \n");
    printf("SYNOPSIS\n");
    printf("\t mymv [OPTION] [SOURCE] [DEST] \n");
}