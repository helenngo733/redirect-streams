#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "redir.h"

//.redir - "cmd param param" -
// 1) break argv[2] into words
// 2) find the absolute path of the command
int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        return 1;
    }

    char absolute_path[1000];
    char* words[1000];

    split(argv[2], words, ' ');

    if (!find_absolute_path(words[0], absolute_path)) {
        printf("Command not found: %s\n", words[0]);
        return 1;
    }

    for (int i = 0; words[i] != NULL; i++) {
        printf("words[%d] = '%s;\n", i, words[i]);
    }
    printf("absolute_path = '%s'\n", absolute_path);

    // input and output file descriptors
    int input_fd;
    if (strcmp(argv[1], "-") == 0) {
        input_fd = STDIN_FILENO;
    } else {
        input_fd = open(argv[1], O_RDONLY);
        if (input_fd == -1) {
            fprintf(stderr, "Failed to open %s\n", argv[1]);
            return 1;
        }
    }

    int output_fd;
    if (strcmp(argv[3], "-") == 0) {
        output_fd = STDOUT_FILENO;
    } else {
        output_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (output_fd == -1) {
            fprintf(stderr, "Failed to open %s\n", argv[3]);
            return 1;
        }
    }

    // Fork and execute
    int child_pid = fork();
    if (child_pid == 0) {
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);  // Redirect input
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);  // Redirect output
            close(output_fd);
        }
        execve(absolute_path, words, NULL);
        fprintf(stderr, "Failed to execute %s\n", absolute_path);
        exit(1);
    }

    close(input_fd);
    close(output_fd);

    wait(NULL);

    return 0;
}