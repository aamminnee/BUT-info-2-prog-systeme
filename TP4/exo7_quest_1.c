#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

int main() {
    int pfd[2];
    pid_t pid;

    srand(time(NULL));

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
        while (1) {
            int r = rand() % 100;
            write(pfd[1], &r, sizeof(int));
            int sleep_time = (rand() % 5) + 1;
            sleep(sleep_time);
        }
    } else {
        close(pfd[1]);
        int flags = fcntl(pfd[0], F_GETFL, 0);
        fcntl(pfd[0], F_SETFL, flags | O_NONBLOCK);

        int val;
        while (1) {
            sleep(1);
            ssize_t n = read(pfd[0], &val, sizeof(int));
            if (n > 0) {
                printf("Père: reçu %d\n", val);
            } else {
                printf("Père: rien de nouveau\n");
            }
        }
    }
    return 0;
}