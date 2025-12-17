#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>

int main() {
    int pfd[2];
    pid_t pid;
    char c;
    int count = 0;

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
        while (read(STDIN_FILENO, &c, 1) > 0) {
            if (count % 2 == 0) {
                c = toupper((unsigned char)c);
            } else {
                c = tolower((unsigned char)c);
            }
            write(pfd[1], &c, 1);
            count++;
        }
        close(pfd[1]);
        exit(0);
    } else {
        close(pfd[1]);
        while (read(pfd[0], &c, 1) > 0) {
            write(STDOUT_FILENO, &c, 1);
        }
        close(pfd[0]);
        wait(NULL);
    }

    return 0;
}