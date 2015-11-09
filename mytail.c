#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_ARRAY_SIZE 128
#define LINE_BUFFER_SIZE 1024

char *read_line(FILE *file);
void print_file_tail(FILE* fp, long num_line);
void print_help();

int main(int argc, char** argv) {
    if(argc<2){
        fprintf(stderr, "mytail : missing operand after 'mytail'\n");
        printf("mytail : Try 'mytail --help' for more information.\n");
        printf("usage : mytail [FILENAME] [NUMBER]\n");
    }else if(argc==2){
        if(strcmp(argv[1], "--help") == 0) {
            print_help();
        }else{
            FILE* fp;

            fp = fopen(argv[1], "r");
            if(fp){
                print_file_tail(fp, 5);
                fclose(fp);
            }else{
                fprintf(stderr, "%s: Cannot open the file.\n", argv[1]);
            }
        }
    }else{
        int i;
        int num_str_len = strlen(argv[2]);
        long read_num_lines;
        FILE* fp;

        for(i=0; i<num_str_len; i++){
            if(isdigit(argv[2][i]) == 0){
                fprintf(stderr, "argument [NUMBER] is only positive interger.");
                printf("usage: mytail file [NUMBER]\n");
                return 1;
            }
        }

        read_num_lines = atol(argv[2]);
        if(read_num_lines < 0l){
            fprintf(stderr, "argument [NUMBER] is only positive interger.");
            printf("usage: mytail file [NUMBER]\n");
            return 1;
        }

        fp = fopen(argv[1], "r");
        if(fp){
            print_file_tail(fp, read_num_lines);
            fclose(fp);
        }else{
            fprintf(stderr, "%s: Cannot open the file.\n", argv[1]);
        }
    }

    return 1;
}

void print_file_tail(FILE* fp, long num_line) {
    int line_cnt = 0;
    char ch;
    while(!feof(fp)) {
        ch = fgetc(fp);
        if(ch == '\n') {
            line_cnt++;
        }
    }
    line_cnt++;

    char ** lines = (char **)malloc(sizeof(char*) * (line_cnt));

    fseek(fp, 0, SEEK_SET);
    for(int i=0; i<line_cnt; i++){
        lines[i] = read_line(fp);
    }

    for(int i=line_cnt-1; i>=line_cnt-num_line; i--){
        printf("%s\n", lines[i]);
        if(i==0) break;
    }

    for(int i=0; i<line_cnt; i++){
        free(lines[i]);
    }
    free(lines);
}

char *read_line(FILE *file) {
    char ch;
    int count = 0;
    int buffer_size = LINE_BUFFER_SIZE;
    char *line_buffer = (char*)malloc(sizeof(char) * buffer_size);

    ch = getc(file);
    if(ch == EOF){
        return NULL;
    }

    while((ch != '\n') && (ch != EOF)){
        if(count == buffer_size){
            buffer_size += LINE_BUFFER_SIZE;
            line_buffer = (char*)realloc(line_buffer, sizeof(char) * buffer_size);
        }

        line_buffer[count] = ch;
        count++;

        ch = getc(file);
    }
    line_buffer[count] = '\0';

    return line_buffer;
}

void print_help() {
    printf("NAME\n");
    printf("\t mytail - display last lines of a file \n");
    printf("SYNOPSIS\n");
    printf("\t mytail [FILENAME] [NUMBER]\n");
    printf("\t[NUMBER]: option, the number of lines to print.(DEFAULT=5)\n");
}