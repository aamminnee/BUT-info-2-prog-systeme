#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void run_command(char *cmd[], const char *name, const char *before, const char *after) {
    printf("%s\n", before);
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(name, cmd) == -1) {
            perror("execvp");
            _exit(1);
        }
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("%s (status = %d)\n", after, WEXITSTATUS(status));
        } else {
            printf("%s (terminé anormalement)\n", after);
        }
    } else {
        perror("fork");
        exit(1);
    }
}

int main() {
    char *ping_cmd[] = {"ping", "-c", "4", "google.com", NULL};
    char *ps_cmd[] = {"ps", NULL};
    char *free_cmd[] = {"free", NULL};
    run_command(ping_cmd, "ping", "Je lance ping", "Ping est fini : je lance ps");
    run_command(ps_cmd, "ps", "Je lance ps", "Ps est fini : je lance free");
    run_command(free_cmd, "free", "Je lance free", "Free est fini");

    return 0;
}
