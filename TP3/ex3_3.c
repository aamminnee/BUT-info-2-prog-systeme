#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    pid_t pid;
    char msg[100];
    for (int i = 1; i <= 20; i++) {
        pid = fork();
        if (pid == 0) {
            // fils
            int len = snprintf(msg, sizeof(msg), 
                "Je suis le numéro %d, mon PID = %d, mon père = %d\n",
                i, getpid(), getppid());
            write(1, msg, len);
            _exit(0); 
        }
    }
    for (int i = 1; i <= 20; i++) {
        waitpid(-1, NULL, 0);
    }
    printf("tous les fils ont terminé.\n");
    return 0;
}
