#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* struct dirent */
#include <dirent.h>

/* struct stat */
#include <sys/stat.h>

#define DIR_SPLIT_DELIM "/"

void print_help();

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "myrm : missing operand after 'myrm'\n");
        printf("myrm : Try 'myrm --help' for more information.\n");
        printf("usage : myrm [OPTION] [FILENAME]\n");
        exit(1);
    } else if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_help();
    } else {
        if (strcmp(argv[1], "-rf") == 0) {
            struct stat st;
            if (stat(argv[2], &st) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    int rm_return = rmdir(argv[2]);
                    if(rm_return != 0){
                        struct dirent* dirent_pointer;
                        struct stat stat_buf;
                        DIR* direct;
                        char* dir_path = argv[2];
                        direct = opendir(dir_path);

                        if ( direct  == NULL) {
                            fprintf(stderr, "myrm: %s: cannot access to directory\n", dir_path);
                        }else {
                            strcat(dir_path, DIR_SPLIT_DELIM);
                            while ((dirent_pointer = readdir(direct)) != NULL){
                                if( strcmp(dirent_pointer->d_name,".") == 0 || strcmp(dirent_pointer->d_name,"..") == 0 )
                                    continue;
                                else{
                                    char* file_path = (char*)malloc(500);
                                    strcat(file_path, dir_path);
                                    char* full_path = strcat(file_path, dirent_pointer->d_name);

                                    if (open(full_path, O_RDWR) < 0) {
                                        printf("myrm : %s: No such file or directory\n", dirent_pointer->d_name);
                                        exit(1);
                                    }
                                    if (remove (full_path) < 0) {
                                        fprintf(stderr, "myrm : %s: remove file failed.\n", dirent_pointer->d_name);
                                    }
                                }
                            }
                            if (rmdir(argv[2]) < 0)
                                fprintf(stderr, "myrm : %s: remove failed.\n", argv[2]);
                            closedir(direct);
                        }
                    }
                } else {
                    if (remove(argv[2]) < 0)
                        fprintf(stderr, "myrm : %s: remove failed.\n", argv[2]);
                    else
                        printf("myrm :  %s: removed successfully.\n", argv[2]);
                }
            } else {
                fprintf(stderr, "myrm : %s: No such file or directory.\n", argv[2]);
                exit(1);
            }
        } else {
            struct stat st;
            if (stat(argv[1], &st) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    fprintf(stderr, "myrm : %s: is a directory\n", argv[1]);
                    exit(1);
                } else {
                    if (open(argv[1], O_RDWR) < 0) {
                        printf("myrm : %s: No such file or directory\n", argv[1]);
                        exit(1);
                    }

                    printf("file path : %s", argv[1]);
                    if (remove(argv[1]) < 0)
                        fprintf(stderr, "myrm : %s: remove file failed.\n", argv[1]);
                    else
                        printf("myrm :  %s: removed successfully.\n", argv[1]);
                }
            } else {
                fprintf(stderr, "myrm : %s: No such file or directory.\n", argv[1]);
                exit(1);
            }
        }
    }
}

void print_help() {
    printf("NAME\n");
    printf("\t myrm - remove directory entries \n");
    printf("SYNOPSIS\n");
    printf("\t myrm [OPTION] [FILENAME] \n");
}