#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

/* struct dirent */
#include <dirent.h>

/* struct stat */
#include <sys/stat.h>

/* struct passwd, group */
#include <pwd.h>
#include <grp.h>

void list_file(char *dir, char *fname);
void print_file_info(struct dirent *dirent_pointer, struct stat *stat_buf);
void print_help();

int main (int argc, char **argv)
{
   if(argc >= 2) {
        if(strcmp(argv[1], "--help") == 0){
              print_help();
        }else {
            struct stat st;
            if (stat(argv[1], &st) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    list_file(argv[1], "");
                } else {
                    /* myls [FILENAME] */
                    list_file(".", argv[1]);
                }
            } else {
                fprintf(stderr, "%s: No such file or directory.\n", argv[1]);
            }
        }
    }else {
        list_file(".", "");
    }
    return 0;
}

void list_file(char *dir, char *filename)
{
    struct dirent *dirent_pointer;
    struct stat stat_buf;
    DIR *direct;
    direct = opendir(dir);

    if ( direct  == NULL) {
        fprintf(stderr, "myls: cannot open %s\n", dir);
        return;
    }else if (strcmp(".", dir) == 0 && strlen(filename) != 0) {
        while ((dirent_pointer = readdir(direct)) != NULL) {
            if (strcmp(dirent_pointer->d_name, filename) == 0) {
                stat(dirent_pointer->d_name, &stat_buf);
                print_file_info(dirent_pointer, &stat_buf);
            }
        }
        closedir(direct);
        return;
    }else {
        while ((dirent_pointer = readdir(direct)) != NULL){
            if( strcmp(dirent_pointer->d_name,".") == 0 || strcmp(dirent_pointer->d_name,"..") == 0 )
                continue;

            stat(dirent_pointer->d_name, &stat_buf);

            print_file_info(dirent_pointer, &stat_buf);

        }
        closedir(direct);
        return;
    }
}

void print_file_info(struct dirent *dirent_pointer, struct stat *stat_buf){
    struct passwd *user_info;
    struct group *group_info;
    char my_time[128];

    /* file type */
    switch(stat_buf->st_mode & S_IFMT){
        case S_IFDIR :  /* directory */
            putchar('d');
            break;
        case S_IFREG :  /* regular file */
            putchar('-');
            break;
        case S_IFBLK :  /* block special device file */
            putchar('b');
            break;
        case S_IFCHR :  /* character special device file */
            putchar('c');
            break;
        case S_IFIFO :  /* FIFO (named pipe) */
            putchar('f');
            break;
        case S_IFSOCK :  /* socket */
            putchar('s');
            break;
        case S_IFLNK :  /* symbolic link */
            putchar('l');
            break;
    }

    /* file permission */
    for(int i=0; i<3; i++){
        /* read */
        if(stat_buf->st_mode & (S_IREAD >> (i*3)))
            putchar('r');
        else
            putchar('-');
        /* write */
        if(stat_buf->st_mode & (S_IWRITE >> (i*3)))
            putchar('w');
        else
            putchar('-');
        /* execute */
        if(stat_buf->st_mode & (S_IEXEC >> (i*3)))
            putchar('x');
        else
            putchar('-');
    }

    /* number of link */
    printf(" %d", stat_buf->st_nlink);

    /* name of user */
    user_info = getpwuid(stat_buf->st_uid);
    printf(" %s", user_info->pw_name);

    /* name of group */
    group_info = getgrgid(stat_buf->st_gid);
    printf(" %s", group_info->gr_name);

    /* file size */
    printf(" %4lld", stat_buf->st_size);

    /* modification time */
    char *temp_time = ctime(&stat_buf->st_mtime);
    strncpy(my_time, temp_time, strlen(temp_time)-1);
    my_time[strlen(temp_time)-1] = '\0';
    printf(" %s", my_time);

    /* file name */
    printf(" %s\n", dirent_pointer->d_name);
}

void print_help() {
    printf("NAME\n");
    printf("\t myls - list directory contents \n");
    printf("SYNOPSIS\n");
    printf("\t myls [FILENAME]\n");
    printf("\t[FILENAME]: option, filename or directory\n");
}