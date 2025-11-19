#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void run_command(char *cmd[], const char *name) {
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(name, cmd) == -1) {
            perror("execvp");
            _exit(1);
        }
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
        exit(1);
    }
}

int main() {
    char *ping_cmd[] = {"ping", "-c", "4", "google.com", NULL};
    char *ps_cmd[] = {"ps", NULL};
    char *free_cmd[] = {"free", NULL};
    run_command(ping_cmd, "ping");
    run_command(ps_cmd, "ps");
    run_command(free_cmd, "free");
    return 0;
}
