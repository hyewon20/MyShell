#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_SIZE 1024
#define DIR_SPLIT_DELIM "/"
#define PATH_COUNT 16

void print_help();

int main(int argc, char** argv) {

    if(argc < 2){
        fprintf(stderr, "mymkdir : missing operand after 'mymkdir'\n");
        printf("mymkdir : Try 'mymkdir --help' for more information.\n");
        printf("usage : mymkdir [DIRECTORY]\n");
        printf("usage : mymkdir -p [PATH]\n");
    }else if(argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            print_help();
        } else {
            if (strlen(argv[1]) > 128) {
                printf("mymkdir : directory not over 128 byte\n");
                exit(1);
            }

            DIR *direct;
            struct dirent *dirent_pointer;

            char dir_path[128];   /* store directory path */
            memset(dir_path, 0, sizeof(dir_path));

            strcpy(dir_path, argv[1]);

            direct = opendir(argv[1]);

            if (direct == NULL) {
                mkdir(dir_path, S_IRWXU | S_IRWXG | S_IRWXO);
            } else { /* already exist */
                fprintf(stderr, "mymkdir :%s : File exists (Already exist directory)\n", argv[1]);
                exit(1);
            }
        }

        struct stat st;
        if (stat(argv[1], &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                printf("mymkdir :%s : successfully create directory\n", argv[1]);
                char cwd[DIR_SIZE];

                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("mymkdir : PATH : %s \n", cwd);
                } else {
                    fprintf(stderr, "mymkdir : get current directory error\n");
                }
            }
        }
    } else { /* with - path option */
        if (argc >= 3 && strcmp(argv[1], "-p") == 0) {

            if (strlen(argv[2]) > 128) {
                printf("mymkdir : directory not over 128 byte\n");
                exit(1);
            }

            DIR *direct;
            struct dirent *dirent_pointer;
            char dir_path[128];   /* store directory path */

            memset(dir_path, 0, sizeof(dir_path));
            strcpy(dir_path, argv[2]);
            direct = opendir(argv[2]);

            if(strstr(argv[2], "/"))
            {
                char* pch, *last_pch;
                char* directory_list[PATH_COUNT];
                char* full_directory_path = (char*)malloc(500);
                int path_count = 0;

                last_pch = NULL;
                pch = strtok(dir_path, DIR_SPLIT_DELIM);

                printf("1st pch : %s\n",pch);

                /* split directory '/' */
                while(pch != NULL) {
                    directory_list[path_count] = pch;
                    pch = strtok(NULL, DIR_SPLIT_DELIM);
                    path_count++;
                }

                printf("path count : %d \n", path_count);

                struct stat st;
                /* check exist directory */
//                int exist_num = 0;
//
//                int i = 0;
//                while(stat(directory_list[i], &st) == 0 && S_ISDIR(st.st_mode)) {
//                        strcat(full_directory_path, directory_list[i]);
//                        exist_num++;
//                        printf("full_directory_path : %s \n",full_directory_path);
//                        i++;
//                }
//
//
//                printf("exist number : %d\n", exist_num);
//
//                for(int i=exist_num; i < path_count; i++){
//                    strcat(dir_path, directory_list[i]);
//                    mkdir(dir_path, S_IRWXU | S_IRWXG | S_IRWXO);
//                }
//            }
        } else{
            printf("mymkdir : Try 'mymkdir --help' for more information.\n");
            printf("usage : mymkdir [DIRECTORY]\n");
            printf("usage : mymkdir -p [PATH]\n");
            exit(1);
        }
    }

    return 0;
}

void print_help() {
    printf("NAME\n");
    printf("\t mymkdir - make directories \n");
    printf("SYNOPSIS\n");
    printf("\t mymkdir [DIRECTORY]\n");
    printf("\t mymkdir -p [PATH]\n");
}