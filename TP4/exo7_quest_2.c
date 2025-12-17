#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
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
        
        fd_set readfds;
        struct timeval tv;
        int val;

        while (1) {
            FD_ZERO(&readfds);
            FD_SET(pfd[0], &readfds);

            tv.tv_sec = 1;
            tv.tv_usec = 0;

            int ret = select(pfd[0] + 1, &readfds, NULL, NULL, &tv);

            if (ret > 0) {
                if (FD_ISSET(pfd[0], &readfds)) {
                    read(pfd[0], &val, sizeof(int));
                    printf("Père: reçu %d\n", val);
                }
            } else if (ret == 0) {
                printf("Père: rien de nouveau (timeout)\n");
            } else {
                perror("select");
            }
        }
    }
    return 0;
}