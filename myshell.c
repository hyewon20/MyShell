/* standard library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
/* GNU library for process */
#include <unistd.h>
#include <sys/wait.h>

#define CMD_LINE_SIZE 1024
#define DIR_SIZE 1024
#define MAX_SPLIT_COUNT 32
#define SPLIT_DELIM " "
#define DIR_SPLIT_DELIM "/"
#define LINE_BUFFER_SIZE 1024

void read_cmd_line(char *cmd_line);
int split_cmd_line(char *args[], char *cmd_line);
int execute(int argc, char **args);

int execute_script(char* filename);
char *read_line(FILE *file);

void get_prompt(char *dir);
void set_path_environ();
void change_dir(int argc, char **args);

int strstart(const char* base, const char* substr);
int strend(const char* base, const char* substr);

int main(int argc, char **argv){
	char cmd_line[CMD_LINE_SIZE];
	char *args[MAX_SPLIT_COUNT];
	char dir[DIR_SIZE];
	int args_count = 0;
	int is_running = 1;

	set_path_environ();

	while(is_running){
		get_prompt(dir);
		printf("mysh> %s$ ", dir);
		read_cmd_line(cmd_line);
		args_count = split_cmd_line(args, cmd_line);
		is_running = execute(args_count, args);
		
		/* release args buffer */
		int i;
		for(i=0; i<args_count; i++) {
			free(args[i]);
		}
	}
}

/* read from command line */
void read_cmd_line(char *cmd_line){
	fgets(cmd_line, CMD_LINE_SIZE, stdin);
	cmd_line[strlen(cmd_line) -1 ] = '\0';	/* remove '\n'*/
}

int split_cmd_line(char *args[], char *cmd_line){
	char *pch;		/* token pointer */
	int count = 0;

	pch = strtok(cmd_line, SPLIT_DELIM);
	while(pch != NULL){
		args[count] = malloc(sizeof(char) * (strlen(pch)+1));
		strcpy(args[count], pch);
		pch = strtok(NULL, SPLIT_DELIM);
		count++;
	}

	return count;
}

int execute(int argc, char **args){
	if(args[0] == NULL){
		return 1;
	}

	if(strcmp(args[0], "exit") == 0){
		printf("Successfully mysh terminated.\n");
		return 0;
	}else if(strcmp(args[0], "cd") == 0){
		change_dir(argc, args);
	}else if(strstart(args[0], "./")) {
        if(strend(args[0], ".sh")){
            return execute_script(args[0]);
        }else{
            fprintf(stderr, "mysh: %s: Cannot run the file\n", args[0]);
        }
    }else{
		pid_t pid, wpid;
		int status;

		pid = fork();
		if(pid == 0 ){
			/* child process */
			if(execvp(args[0], args) == -1){
                fprintf(stderr, "mysh: %s: Command not found\n", args[0]);
			}
			exit(EXIT_FAILURE);
		}else if(pid < 0){
			/* fork error */
			fprintf(stderr, "fork error\n");
		}else{
			/* parent process */
			do{
				wpid = waitpid(pid, &status, WUNTRACED);
			}while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}

	return 1;
}

int execute_script(char* filename) {
    FILE* fp;

    fp = fopen(filename, "r");
    if(fp){
        char *line = NULL;
        char *args[MAX_SPLIT_COUNT];
        int args_count = 0;
        int is_running = 1;

        line = read_line(fp);
        while(line){
			is_running = 1;
            if(!strstart(line, "#")) {
                args_count = split_cmd_line(args, line);
                is_running = execute(args_count, args);

                /* release args buffer */
                int i;
                for (i = 0; i < args_count; i++) {
                    free(args[i]);
                }
            }

            /* release line buffer */
            free(line);

            if(is_running == 0) {
                return 0;
            }

            line = read_line(fp);
        }

    }else{
        fprintf(stderr, "mysh: %s: Cannot open the script.\n", filename);
    }

    return 1;
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

void get_prompt(char *dir){
	char cwd[DIR_SIZE];
	char* pch, *last_pch;

	if(getcwd(cwd, sizeof(cwd)) != NULL){
		last_pch = NULL;
		pch = strtok(cwd, DIR_SPLIT_DELIM);
		while(pch != NULL){
			last_pch = pch;
			pch = strtok(NULL, DIR_SPLIT_DELIM);
		}
		
		strcpy(dir, "20122086 ");
		if(last_pch == NULL) {
			strcat(dir, "/");
		} else {
			strcat(dir, last_pch);
		}
	}else{
		fprintf(stderr, "get prompt error\n");
	}
}

void set_path_environ() {
	char cwd[DIR_SIZE];
	char env_str[1024];

	char *env_path = getenv("PATH");

	if(getcwd(cwd, sizeof(cwd)) != NULL) {
		if(strstr(env_path, cwd) == NULL) {
		/* current directory not exist in environment variable, then add envron */
			sprintf(env_str, "%s:%s", env_path, cwd);
			setenv("PATH", env_str, 1);
		}
	}else{
		fprintf(stderr, "get prompt error\n");
	}
}

void change_dir(int argc, char **args){
	if(argc == 1){
		chdir("/home");
	}else if(argc >= 2){
		if(strcmp(args[1], "~") == 0){
			chdir("/home");
		}else{
			struct stat st;
			if(stat(args[1], &st) == 0) {
				if(S_ISDIR(st.st_mode)) {
					chdir(args[1]);
				} else {
					fprintf(stderr, "%s: Not a directory.\n", args[1]);
				}
			} else {
				fprintf(stderr, "No such file or directory.\n");
			}
		}
	}
}

int strstart(const char* base, const char* substr) {
    return (strstr(base, substr) - base) == 0;
}

int strend(const char* base, const char* substr) {
    int blen = strlen(base);
    int slen = strlen(substr);
    return (blen >= slen) && (0 == strcmp(base + blen - slen, substr));
}
