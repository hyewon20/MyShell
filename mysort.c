#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_ARRAY_SIZE 128
#define LINE_BUFFER_SIZE 1024

char *read_line(FILE *file);
int compare(const void *a , const void *b);
void print_file_sort(FILE* fp);
void print_help();

int main(int argc, char** argv) {
    if(argc<2){
        fprintf(stderr, "mysort : missing operand after 'mysort'\n");
        printf("mysort : Try 'mysort --help' for more information.\n");
        printf("usage : mysort [FILENAME]\n");
    }else{
        if(strcmp(argv[1], "--help") == 0) {
            print_help();
        } else {
            FILE* fp;

            fp = fopen(argv[1], "r");
            if(fp) {
                print_file_sort(fp);
                fclose(fp);
            } else {
                fprintf(stderr, "%s: Cannot open the file.\n", argv[1]);
            }
        }
    }

    return 1;
}

void print_file_sort(FILE* fp) {
    int line_cnt = 0;
    int i;
    char ch;
    while(!feof(fp)) {
        ch = fgetc(fp);
        if(ch == '\n') {
            line_cnt++;
        }
    }
    line_cnt++;

    char* lines[line_cnt];

    fseek(fp, 0, SEEK_SET);
    for(i=0; i<line_cnt; i++){
        lines[i] = read_line(fp);
    }

    qsort((void*)lines, line_cnt, sizeof(char*), compare);

    for(i=0; i<line_cnt; i++){
        printf("%s\n", lines[i]);
    }

    for(i=0; i<line_cnt; i++){
        free(lines[i]);
    }

};

int compare(const void *a , const void *b){
    return (strcmp(*(const char**)a , *(const char**)b));
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
    printf("\t mysort - sort lines of text files \n");
    printf("SYNOPSIS\n");
    printf("\t mysort [FILENAME]\n");
}