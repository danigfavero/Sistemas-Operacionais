#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int bool;
#define true 1
#define false 0

/*
    Simple shell made for MINIX system calls. There are for options to use it:

    1. 'liberageral <file-path>' : gives 777 permission to the file;
    2. 'protegepracaramba <file-path>' : gives 000 permission to the file;
    3. 'rodeveja <file-path>' : executes the file and print returned status;
    4. 'rode <file-path>' : executes the file in background.

    Made by Daniela Favero and Felipe Noronha at IME-USP.
*/

/* checks if file exists or if it exists and is executable/locked, according
   to the given mode */
bool valid_file(char *path, int mode) {

    if (access(path, mode) != -1)
        return true;
    printf("Error: file not found!\n");
    return false;
}

/* removes line feed character from the end of char* */
void remove_lf(char *path) {

    path[strlen(path)-1] = '\0';
}

/* counts words of a given string */
int word_count(char *str) {

    int state = 0, count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
            state = 0;
        else if (state == 0) {
            state = 1;
            count++;
        }
    }
    return count;
}

/* make given file receive 000 protection */
void protegepracaramba(char *cmd, char *path) {

    int res;
    res = chmod(path, 0000);
    if (res == -1)
        fprintf(stderr, "chmod failed, errno = %d\n", res);
}

/* make given file receive 000 protection */
void liberageral(char *cmd, char *path) {

    int res;
    res = chmod(path, 0777);
    if (res == -1)
        fprintf(stderr, "chmod failed, errno = %d\n", res);
}

/* executes the given program and print the returned value in the stdout */
void rodeveja(char *cmd, char *path) {

    if (!valid_file(path, X_OK)) {
        printf("Error while executing file: permission denied!\n");
        return;
    }

    pid_t pid;
    pid = fork();

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

/* executes the given program in background, while out shell keep in commmand
   of the stdin */
void rode(char *cmd, char *path) {

    if (!valid_file(path, X_OK)) {
        printf("Error while executing file: permission denied!\n");
        return;
    }

    pid_t pid;
    pid = fork();

    if (pid == 0) {
        char* argv[] = {NULL};
        char* envp[] = {NULL};
        fclose(stdin);
        if (execve(path, argv, envp) == -1)
            printf("Error executing file!\n");
    }
}

/* given a command (cmd) and a file-path (path) executes the respective
   function on the specified file  */
void process(char *cmd, char *path) {

    if (cmd == NULL || path == NULL) {
        printf("Error while processing: invalid syntax!\n");
        return;
    }

    remove_lf(path);

    if (!valid_file(path, F_OK)) {
        printf("Error while processing: file does not exist!\n");
        return;
    }

    if (strcmp(cmd, "protegepracaramba") == 0)
        protegepracaramba(cmd, path);

    else if (strcmp(cmd, "liberageral") == 0)
        liberageral(cmd, path);

    else if (strcmp(cmd, "rodeveja") == 0)
        rodeveja(cmd, path);

    else if (strcmp(cmd, "rode") == 0)
        rode(cmd, path);

    else {
        fprintf(stderr, "Error while processing: invalid syntax!\n");
        return;
    }
}

/* split the received char array into two words, the command and the
   path  */
char **parse_arg(char *arg) {

    int i;
    char *token;
    char **parse = malloc(2*sizeof(char*));

    if (parse == NULL) {
        fprintf(stderr, "Error in parse_arg(): could not allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    if (word_count(arg) != 2) {
        parse[0] = NULL;
        parse[1] = NULL;
        free(token);
        return parse;
    }

    token = strtok(arg, " ");
    for (i = 0; i < 2; i++) {
        parse[i] = malloc((strlen(token)+1)*sizeof(char));
        if (parse[i] == NULL) {
            fprintf(stderr, "Error in parse_arg(): could not allocate memory!\n");
            exit(EXIT_FAILURE);
        }
        strcpy(parse[i], token);
        parse[i][strlen(token)] = '\0';
        token = strtok(NULL, " ");
    }

    free(token);
    return parse;
}

int main(int argc, char **argv) {

    char *buffer = malloc(sizeof(char) * 256);
    if (buffer == NULL) {
        fprintf(stderr, "Error in main(): could not allocate buffer\n");
        exit(EXIT_FAILURE);
    }

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

    /* waiting children processes to end  */
    while (wait(NULL))

    free(buffer);
    return 0;
}
