#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long get_line_number(FILE* fp);
long get_file_size(FILE* fp);
void print_file_compare(FILE* fp, FILE* fp2);
void print_help();

int main(int argc, char** argv) {
    if(argc<2){
        fprintf(stderr, "mycmp : missing operand after 'mycmp'\n");
        printf("mycmp : Try 'mycmp --help' for more information.\n");
        printf("usage : mycmp [FILE1] [FILE2] \n");
    }else if(argc==2){
        if(strcmp(argv[1], "--help") == 0) {
            print_help();
        }else{
            fprintf(stderr, "mycmp : missing operand after 'mycmp'\n");
            printf("mycmp : Try 'mycmp --help' for more information.\n");
            printf("usage : mycmp [FILE1] [FILE2] \n");
        }
    }else{
        FILE* fp = NULL;
        FILE* fp2 = NULL;

        fp = fopen(argv[1], "r");
        fp2 = fopen(argv[2], "r");

        if(fp && fp2) {
            printf("%s %s differ : \n", argv[1], argv[2]);
            print_file_compare(fp, fp2);
            fclose(fp);
            fclose(fp2);
        } else {
            if(fp) fclose(fp);
            if(fp2) fclose(fp2);
        }
    }

    return 1;
}

void print_file_compare(FILE* fp, FILE* fp2) {
    printf("size %ld, line %ld\n", get_file_size(fp),get_line_number(fp));
    printf("size %ld, line %ld\n", get_file_size(fp2),get_line_number(fp2));
}

long get_line_number(FILE* fp) {
    int line_cnt = 0;
    char ch;

    fseek(fp, 0, SEEK_SET);
    while(!feof(fp)) {
        ch = fgetc(fp);
        if(ch == '\n') {
            line_cnt++;
        }
    }
    line_cnt++;
    fseek(fp, 0, SEEK_END);

    return line_cnt;
}

long get_file_size(FILE* fp) {
    long size = 0;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

void print_help(){
    printf("NAME\n");
    printf("\t mycmp - compare two files byte by byte \n");
    printf("SYNOPSIS\n");
    printf("\t mycmp [FILE1] [FILE2] \n");
}