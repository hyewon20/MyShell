#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_SIZE 1024

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
    }

    else { /* with - path option */
        if (argc >= 3 && strcmp(argv[1], "-p") == 0) {

            if (strlen(argv[2]) > 128) {
                printf("mymkdir : directory not over 128 byte\n");
                exit(1);
            }

            DIR *direct;
            struct dirent *dirent_pointer;
            char dir_path[128];   /* store directory path */

            memset(dir_path, 0, sizeof(dir_path));
            strcpy(dir_path, argv[1]);
            direct = opendir(argv[1]);

            // Check if user supplied '/' at the end of directory name.
            // Based on it create a buffer containing path to new directory name 'newDir'
            if(strstr(argv[2],'/'))
            {
                strncpy(dir_path+strlen(dir_path),"newDir/",7);
            } else
            {
                mkdir(buff,S_IRWXU|S_IRWXG|S_IRWXO);
            }

            printf("\n Creating a new directory [%s]\n",buff);
            // create a new directory
            mkdir(buff,S_IRWXU|S_IRWXG|S_IRWXO);
            printf("\n The contents of directory [%s] are as follows \n",argv[1]);
            // Read the directory contents
            while(NULL != (dptr = readdir(dp)) )
            {
                printf(" [%s] ",dptr->d_name);
            }
            // Close the directory stream
            closedir(dp);
            // Remove the new directory created by us
            rmdir(buff);
            printf("\n");
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