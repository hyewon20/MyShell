/* standard library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* GNU library for process */
#include <unistd.h>


#define CMD_LINE_SIZE 1024
#define DIR_SIZE 1024
#define MAX_SPLIT_COUNT 32
#define SPLIT_DELIM " "
#define DIR_SPLIT_DELIM "/"

void read_cmd_line(char *cmd_line);
int split_cmd_line(char *args[], char *cmd_line);
int execute(int argc, char **args);

void get_prompt(char *dir);
void set_path_environ();
void change_dir(int argc, char **args);

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
	char *line = NULL;
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
		exit(1);
	}else if(strcmp(args[0], "cd") == 0){
		change_dir(argc, args);
	}else{
		pid_t pid, wpid;
		int status;

		pid = fork();
		if(pid == 0 ){
			/* child process */
			if(execv(args[0], args) == -1){
				fprintf(stderr, "command not found\n");
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
		//TODO Change
		//chdir("/");
		//strcpy(dir, "20122086 /");
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
