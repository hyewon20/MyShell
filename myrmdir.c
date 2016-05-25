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
        fprintf(stderr, "myrmdir : missing operand after 'myrmdir'\n");
        printf("myrmdir : Try 'myrmdir --help' for more information.\n");
        printf("usage : myrmdir [DIRECTORY]\n");
        exit(1);
    } else if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_help();
    } else {
        struct stat st;
        if (stat(argv[1], &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                int rm_return = rmdir(argv[1]);
                if(rm_return != 0){
                    struct dirent* dirent_pointer;
                    DIR* direct;
                    char* dir_path = argv[1];
                    direct = opendir(dir_path);

                    if ( direct  == NULL) {
                        fprintf(stderr, "myrmdir: %s: cannot access to directory\n", dir_path);
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
                                    printf("myrmdir : %s: No such directory\n", dirent_pointer->d_name);
                                    exit(1);
                                }
                                if (remove (full_path) < 0) {
                                    fprintf(stderr, "myrmdir : %s: remove failed.\n", dirent_pointer->d_name);
                                }
                            }
                        }
                        if (rmdir(argv[1]) < 0)
                            fprintf(stderr, "myrmdir : %s: remove failed.\n", argv[1]);
                        closedir(direct);
                    }
                }
            } else { /* [DIRECTORY] is not a directory (not allowed file remove using myrmdir)*/
                    fprintf(stderr, "myrmdir : %s: No such directory.\n", argv[1]);
                    exit(1);
            }
        } else {
            fprintf(stderr, "myrmdir : %s: No such directory.\n", argv[1]);
            exit(1);
        }
    }
}
void print_help() {
    printf("NAME\n");
    printf("\t myrmdir - remove directories \n");
    printf("SYNOPSIS\n");
    printf("\t myrmdir [DIRECTORY] \n");
}
