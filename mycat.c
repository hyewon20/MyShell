#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_file_content(FILE* fp, long num_byte);
void print_help();

int main(int argc, char** argv) {
    if(argc<2){
        fprintf(stderr, "mycat : missing operand after 'mycat'\n");
        printf("mycat : Try 'mycat --help' for more information.\n");
        printf("usage : mycat [FILENAME] [NUMBER]\n");
    }else if(argc==2){
        if(strcmp(argv[1], "--help") == 0) {
            print_help();
        }else{
            FILE* fp;

            fp = fopen(argv[1], "r");
            if(fp){
                print_file_content(fp, -1);
                fclose(fp);
            }else{
                fprintf(stderr, "%s: %s: No such file or directory.\n", argv[0], argv[1]);
            }
        }
    }else{
        int i;
        int num_str_len = strlen(argv[2]);
        long read_num_byte;
        FILE* fp;

        for(i=0; i<num_str_len; i++){
            if(isdigit(argv[2][i]) == 0){
                fprintf(stderr, "argument [number] is only positive interger.");
                printf("usage: mycat file [number]\n");
                return 1;
            }
        }

        read_num_byte = atol(argv[2]);
        if(read_num_byte < 0l){
            fprintf(stderr, "argument [number] is only positive interger.");
            printf("usage: mycat file [number]\n");
            return 1;
        }

        fp = fopen(argv[1], "r");
        if(fp){
            print_file_content(fp, read_num_byte);
            fclose(fp);
        }else{
            fprintf(stderr, "%s: Cannot open the file.\n", argv[1]);
        }
    }

    return 1;
}

void print_file_content(FILE* fp, long num_byte) {
    char buffer[1];
    int i;

    if(num_byte == -1l) {
        /* get file size */
        fseek(fp, 0, SEEK_END);
        num_byte = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }

    for(i=0; i<num_byte; i++){
        if(fread(buffer, sizeof(char), 1, fp) > 0) {
            putchar(buffer[0]);
        }else{
            break;
        }
    }

    printf("\n");
}

void print_help(){
    printf("NAME\n");
    printf("\t mycat - concatenate and print files \n");
    printf("SYNOPSIS\n");
    printf("\t mycat [FILENAME] [NUMBER]\n");
    printf("\t[NUMBER]: option, the number of byte to print.\n");
}