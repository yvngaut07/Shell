#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length of a command */

// Function prototypes
void parseInput(char *input, char **args);
void executeCommand(char **args);

int main() {
    char input[MAX_LINE]; // Buffer to hold the input command
    char *args[MAX_LINE / 2 + 1]; // Array to hold command arguments
    int should_run = 1; // Flag to determine when to exit the shell

    while (should_run) {
        printf("shell> ");
        fflush(stdout);

        if (fgets(input, MAX_LINE, stdin) == NULL) {
            perror("fgets");
            exit(1);
        }

        // Remove the newline character
        input[strcspn(input, "\n")] = 0;

        // Parse the input into command and arguments
        parseInput(input, args);

        // Check for the 'exit' command
        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
        } else {
            // Execute the command
            executeCommand(args);
        }
    }
    return 0;
}

void parseInput(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

void executeCommand(char **args) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(1);
    } else {
        // Parent process
        wait(NULL);
    }
}
