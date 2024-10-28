#include "redir.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

int redir(char* input_file, char* cmd, char* output_file) {
    int input_fd;
    if (strcmp(input_file, "-") == 0) {
        input_fd = STDIN_FILENO;
    } else {
        input_fd = open(input_file, O_RDONLY);
        if (input_fd == -1) {
            fprintf(stderr, "Failed to open %s\n", input_file);
            return 1;
        }
    }

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_fd == -1) {
        fprintf(stderr, "Failed to open %s\n", output_file);
        return 1;
    }

    // Split cmd into arguments
    char** new_argv = (char**)malloc(sizeof(char*) * (strlen(cmd) + 1));

    // Tokenize
    int arg_count = 0;
    char* token = strtok(cmd, " ");
    while (token != NULL) {
        new_argv[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    new_argv[arg_count] = NULL;

    // Fork and execute
    int child_pid = fork();
    if (child_pid == 0) {
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        execvp(new_argv[0], new_argv);
        fprintf(stderr, "Failed to execute %s\n", new_argv[0]);
        exit(1);
    }

    close(input_fd);
    close(output_fd);
    free(new_argv);

    wait(NULL);

    return 0;
}
