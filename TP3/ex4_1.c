#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


int main() {
    char *cmd[] = {"ping", "-c", "4", "google.com", NULL};
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp("ping", cmd) == -1) {
            perror("execvp");
            _exit(1);
        }
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
        exit(1);
    }
    return 0;
}
