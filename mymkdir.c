#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_SIZE 1024
#define DIR_SPLIT_DELIM "/"
#define PATH_COUNT 32

void make_directory(char* dir_name);
void print_mkdir_result();
void print_help();

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "mymkdir : missing operand after 'mymkdir'\n");
        printf("mymkdir : Try 'mymkdir --help' for more information.\n");
        printf("usage : mymkdir [DIRECTORY]\n");
        printf("usage : mymkdir -p [PATH]\n");
    } else if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            print_help();
        } else {
            if(strchr(argv[1], '/') == NULL) {
                make_directory(argv[1]);
                print_mkdir_result();
            } else {
                /* without path option but there is a '/' character */
                fprintf(stderr, "mymkdir :%s : Naming Error (Cannot use the character '/' for directory name)\n", argv[1]);
                exit(1);
            }
        }
    } else { /* with - path option */
        if (argc >= 3 && strcmp(argv[1], "-p") == 0) {
            char *pch;		/* token pointer */
            int count = 0;
            char *path[PATH_COUNT];

            pch = strtok(argv[2], DIR_SPLIT_DELIM);
            while(pch != NULL){
                path[count] = malloc(sizeof(char) * (strlen(pch)+1));
                strcpy(path[count], pch);
                pch = strtok(NULL, DIR_SPLIT_DELIM);
                count++;
            }

            struct stat st;
            int success = 1;
            for(int i=0; i<count; i++) {
                if(stat(path[i], &st) == 0) {
                    if(S_ISDIR(st.st_mode)) {
                        chdir(path[i]);
                    } else {
                        fprintf(stderr, "mymkdir :%s : File exists (Already exist a same named file)\n", path[i]);
                        success = 0;
                        break;
                    }
                } else {
                    make_directory(path[i]);
                }
            }

            for(int i=0; i<count; i++) {
                free(path[i]);
            }

            if(success) {
                print_mkdir_result();
            } else {
                exit(1);
            }

        } else {
            printf("mymkdir : Try 'mymkdir --help' for more information.\n");
            printf("usage : mymkdir [DIRECTORY]\n");
            printf("usage : mymkdir -p [PATH]\n");
            exit(1);
        }
    }

    return 1;
}

void make_directory(char* dir_name) {
    if (strlen(dir_name) > 128) {
        fprintf(stderr, "mymkdir : directory not over 128 byte\n");
        exit(1);
    }

    DIR *direct;

    char dir_path[DIR_SIZE];   /* store directory path */
    memset(dir_path, 0, sizeof(dir_path));

    strcpy(dir_path, dir_name);

    direct = opendir(dir_name);

    if (direct == NULL) {
        mkdir(dir_path, S_IRWXU | S_IRWXG | S_IRWXO);
        chdir(dir_name);
    } else { /* already exist */
        fprintf(stderr, "mymkdir :%s : File exists (Already exist directory)\n", dir_name);
        exit(1);
    }
}

void print_mkdir_result() {
    printf("mymkdir : successfully create directory\n");

    char cwd[DIR_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("mymkdir : path : %s \n", cwd);
    } else {
        fprintf(stderr, "mymkdir : get made directory error\n");
    }
}

void print_help() {
    printf("NAME\n");
    printf("\t mymkdir - make directories \n");
    printf("SYNOPSIS\n");
    printf("\t mymkdir [DIRECTORY]\n");
    printf("\t mymkdir -p [PATH]\n");
}
