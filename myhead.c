#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUFFER_SIZE 1024

const char *read_line(FILE *file);
void print_file_head(FILE* fp, long num_line);
void print_help();

int main(int argc, char** argv) {
    if(argc < 2){
        fprintf(stderr, "myhead : missing operand after 'myhead'\n");
        printf("myhead : Try 'myhead --help' for more information.\n");
        printf("usage : myhead [FILENAME] [NUMBER]\n");
    }else if(argc==2){
        if(strcmp(argv[1], "--help") == 0) {
            print_help();
        }else{
            FILE* fp;

            fp = fopen(argv[1], "r");
            if(fp){
                print_file_head(fp, 5);
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
                fprintf(stderr, "argument [number] is only positive interger.");
                printf("usage: myhead file [number]\n");
                return 1;
            }
        }

        read_num_lines = atol(argv[2]);
        if(read_num_lines < 0l){
            fprintf(stderr, "argument [number] is only positive interger.");
            printf("usage: myhead file [number]\n");
            return 1;
        }

        fp = fopen(argv[1], "r");
        if(fp){
            print_file_head(fp, read_num_lines);
            fclose(fp);
        }else{
            fprintf(stderr, "%s: Cannot open the file.\n", argv[1]);
        }
    }

    return 1;
}

void print_file_head(FILE* fp, long num_line) {
    const char *line = NULL;
    int i;

    for(i=0; i<num_line; i++){
        line = read_line(fp);
        if(line == NULL) break;

        printf("%s\n", line);
    }
}

const char *read_line(FILE *file) {
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

    char line[count + 1];
    strncpy(line, line_buffer, (count + 1));
    free(line_buffer);

    const char *const_line = line;
    return const_line;
}

void print_help() {
    printf("NAME\n");
    printf("\t myhead - display first lines of a file \n");
    printf("SYNOPSIS\n");
    printf("\t myhead [FILENAME] [NUMBER]\n");
    printf("\t[NUMBER]: option, the number of lines to print.(DEFAULT=5)\n");
}
