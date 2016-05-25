#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void print_help();

int main(int argc, char** argv) {
    int fd_src, fd_dest;    /* source file and dest file's file descriptor */
    char src_name[128], dest_name[128];
    int f_num;
    char buffer[6];

    if(argc<3){
        if(argc==2 && strcmp(argv[1], "--help") == 0) {
            print_help();
        }else{
            fprintf(stderr, "mycp : missing operand after 'mycp'\n");
            printf("mycp : Try 'mycp --help' for more information.\n");
            printf("usage : mycp [OPTION] [SOURCE] [DEST]\n");
        }
    }else {
        /* limit file name length not larger than 128 */
        if(strlen(argv[1]) > 128 || strlen(argv[2]) > 128) {
            printf("mycp : filename not over 128 byte\n");
            exit(1);
        }else if(strcmp(argv[1], argv[2]) == 0){
            printf("mycp : %s and %s are identical (not copied).\n",argv[1], argv[2] );
            exit(1);
        }

        strcpy(src_name, argv[1]);
        strcpy(dest_name, argv[2]);

        fd_src = open(src_name, O_RDONLY);
        if(fd_src == -1) {
            fprintf(stderr, "mycp :%s : open file error\n", src_name);
            exit(1);
        }

        fd_dest = open(dest_name, O_CREAT| O_WRONLY | O_TRUNC, 0644);
        if(fd_dest == -1) {
            fprintf(stderr, "mycp :%s : copy file open errorr\n", dest_name);
            exit(1);
        }

        /* copy from [SOURCE] to [DEST] 4byte, when copy finished f_num returns 0 then while loop terminated */
        while((f_num = read(fd_src, buffer, 4)) > 0) {
            if(write(fd_dest, buffer, f_num) != f_num)
                fprintf(stderr, "mycp :%s :  file write errorr\n", dest_name);
        }

        if(f_num == -1) {
            fprintf(stderr, "mycp :%s :  file read errorr\n", src_name);
        }

        close(fd_src);
        close(fd_dest);
    }

    return 0;
}

void print_help() {
    printf("NAME\n");
    printf("\t mycp - copy files \n");
    printf("SYNOPSIS\n");
    printf("\t mycp [OPTION] [SOURCE] [DEST] \n");
}