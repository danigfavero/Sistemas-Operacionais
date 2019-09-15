#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>


typedef int bool;
#define true 1
#define false 0

// critical error dealing
#define ERROR(...) { \
        fprintf(stderr, __VA_ARGS__); \
        exit(EXIT_FAILURE); \
}

// checks if file exists or if it exists and is executable
bool valid_file(char *path, int mode) {

    if (access(path, mode) != -1)
        return true;
    printf("Error: file not found!\n");
    return false;
}

// removes line feed character from the end of char*
void remove_lf(char *path) {

    path[strlen(path)-1] = '\0';
}

// counts words of a given string
int word_count(char *str) {
    int count = 1, i;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ' && str[i+1] != ' ')
            count++;    
    }  
    return count;
}

// faz com que o arquivo dado como parâmetro tenha proteção 000
void protegepracaramba(char *cmd, char *path) {

    int res;
    res = chmod(path, 0000);
    if (res == -1)
        fprintf(stderr, "chmod failed, errno = %d\n", res);
}

//  faz com que o arquivo dado como parâmetro tenha proteção 777
void liberageral(char *cmd, char *path) {

    int res;
    res = chmod(path, 0777);
    if (res == -1)
        fprintf(stderr, "chmod failed, errno = %d\n", res);
}

// executa o programa indicado e emite uma mensagem indicando o código
// de retorno
void rodeveja(char *cmd, char *path) {

    if (!valid_file(path, X_OK)) return;
    pid_t pid = fork();

    if (pid == 0) {
        char* argv[] = {NULL}; 
        char* envp[] = {NULL};
        if (execve(path, argv, envp) == -1)
            printf("Error executing file!\n");    
    }

    else {
        int status;
        if (waitpid(pid, &status, 0) > 0)
            printf("programa ‘%s’ retornou com código %d\n",
                    path, WEXITSTATUS(status));
        else
            printf("Error while waiting for the child process to end!\n");
    }
}

// executa o programa indicado em background 
void rode(char *cmd, char *path) {

    if (!valid_file(path, X_OK)) return;
    pid_t pid = fork();

    if (pid == 0) {
        char* argv[] = {NULL}; 
        char* envp[] = {NULL};
        fclose(stdin);
        if (execve(path, argv, envp) == -1)
            printf("Error executing file!\n");   
    }
}

// given a command (cmd) and a file-path (path) executes the respective
// function on the specified file
void process(char *cmd, char *path) {

    if (cmd == NULL || path == NULL) {
        printf("Error while processing: invalid syntax!\n");
        return;
    }    

    remove_lf(path);

    if (strcmp(cmd, "protegepracaramba") == 0)
        protegepracaramba(cmd, path);

    else if (strcmp(cmd, "liberageral") == 0)
        liberageral(cmd, path);

    else if (strcmp(cmd, "rodeveja") == 0)
        rodeveja(cmd, path);

    else if (strcmp(cmd, "rode") == 0)
        rode(cmd, path);

    else {
        ERROR("Error while processing: invalid syntax!\n");
        return;
    }    
}

// split the received char array into two words, the command and the
// path
char **parse_arg(char *arg) {

    char *token;
    char **parse = malloc(2*sizeof(char*));

    if (parse == NULL)
        ERROR("Error in parse_arg(): could not allocate memory!\n");

    if (word_count(arg) != 2) {
        parse[0] = NULL;
        parse[1] = NULL;
        free(token);
        return parse;
    }    

    token = strtok(arg, " ");
    for (int i = 0; i < 2; i++) {
        parse[i] = malloc((strlen(token)+1)*sizeof(char));
        if (parse[i] == NULL)
            ERROR("Error in parse_arg(): could not allocate memory!\n");
        strcpy(parse[i], token);
        parse[i][strlen(token)] = '\0';
        token = strtok(NULL, " ");
    }

    free(token);
    return parse;
}

int main(int argc, char **argv) {

    char *buffer = malloc(sizeof(char) * 256);
    if (buffer == NULL)
        ERROR("Error in main(): could not allocate buffer\n");

    printf("> ");
    while (fgets(buffer, 256, stdin) != NULL){
        char **parse;
        if (buffer[0] == '\n') {
            printf("> ");
            continue;
        }
        parse = parse_arg(buffer);
        process(parse[0], parse[1]);
        printf("> ");
        free(parse);
    }

    // waiting children processes to end
    while (wait(NULL))

    free(buffer);
    return 0;
}