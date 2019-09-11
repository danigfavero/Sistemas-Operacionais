#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

typedef int bool;
#define true 1
#define false 0

#define ERROR(...) { \
        fprintf(stderr, __VA_ARGS__); \
        exit(EXIT_FAILURE); \
}

bool valid_file(char *path) {
    // File exists and is executable by the calling process's
    execve("oi", NULL, NULL);
    if (access(path, X_OK) != -1)
        return true;
    printf("Error: file not found!\n");
    return false;
}

char *path_root(char *path) {
    return path;
}

void process(char *cmd, char *path) {

    if (cmd == NULL || path == NULL)
        ERROR("Error while processing: invalid syntax!\n");

    path = path_root(path);

    path = strcat(path, "\0");

    int i = 0;
    while (path[i] != '\n') i++;
    path[i] = '\0';

    if (!valid_file(path))
        return;

    if (strcmp(cmd, "protegepracaramba") == 0) {
        int res;
        res = chmod(path, 000);
        if (res == -1)
            fprintf(stderr, "chmod failed, errno = %d\n", res);
    }

    else if (strcmp(cmd, "liberageral") == 0) {
        int res;
        res = chmod(path, 777);
        if (res == -1)
            fprintf(stderr, "chmod failed, errno = %d\n", res);
    }

    else if (strcmp(cmd, "rodeveja") == 0){

        pid_t pid = fork();
        if (pid == 0)
            if (execve(path, (char **)NULL, NULL) == -1)
                printf("Error executing file!\n");
        else {
            int status;
            int pid = wait(&status);
            if (pid == -1)
                printf("Error while waiting for the child process to end!\n");
            else
                printf("programa ‘%s’ retornou com código %d", path, WEXITSTATUS(status));
        }
    }

    else if (strcmp(cmd, "rode") == 0) {

    }

    else
        ERROR("Error while processing: invalid syntax!\n");
}


char **parse_arg(char *arg) {

    char *token, *ptr;
    char **parse = malloc(2*sizeof(char*));

    if (parse == NULL)
        ERROR("Error in parse_arg(): could not allocate memory!\n");

    token = strtok(arg, " ");
    for (int i = 0; i < 2; i++) {
        parse[i] = malloc((strlen(token)+1)*sizeof(char));
        if (parse[i] == NULL)
            ERROR("Error in parse_arg(): could not allocate memory!\n");
        strcpy(parse[i], token);
        parse[i][strlen(token)] = '\0';
        token = strtok(NULL, " ");
    }

    return(parse);
}

int main(int argc, char **argv) {

    char *buffer = malloc(sizeof(char) * 256);
    if (buffer == NULL)
        ERROR("Error in main(): could not allocate buffer\n");

    printf("$ ");
    while (fgets(buffer, 256, stdin) != NULL){
        char **parse;
        if (buffer[0] == '\n') {
            printf("$ ");
            continue;
        }
        parse = parse_arg(buffer);
        process(parse[0], parse[1]);
        printf("$ ");
    }

    free(buffer);
    return 0;
}
