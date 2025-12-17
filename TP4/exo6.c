#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

#define BUF_SIZE 256

int main() {
    int pfd[2];
    pid_t pid;

    if (pipe(pfd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        close(pfd[0]);
        char buffer[BUF_SIZE];
        ssize_t n;
        while ((n = read(STDIN_FILENO, buffer, BUF_SIZE - 1)) > 0) {
            buffer[n] = '\0';
            int val = atoi(buffer);
            write(pfd[1], &val, sizeof(int));
        }
        close(pfd[1]);
        exit(0);
    } else {
        close(pfd[1]);
        int val;
        int min = INT_MAX;
        int max = INT_MIN;
        int first = 1;

        while (read(pfd[0], &val, sizeof(int)) > 0) {
            if (first) {
                min = val;
                max = val;
                first = 0;
            } else {
                if (val < min) min = val;
                if (val > max) max = val;
            }
        }
        printf("min = %d\n", min);
        printf("max = %d\n", max);
        close(pfd[0]);
        wait(NULL);
    }
    return 0;
}