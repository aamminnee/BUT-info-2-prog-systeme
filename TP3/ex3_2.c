#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main() {
    pid_t pid;
    char msg[100];
    for (int i = 1; i <= 20; i++) {
        pid = fork();
        if (pid == 0) {
            int len = snprintf(msg, sizeof(msg), 
                "Je suis le numéro %d, mon PID = %d, mon père = %d\n",
                i, getpid(), getppid());
            write(1, msg, len);
            _exit(0);
        }
    }
    while (1) {
        pause();  
    }
    return 0;
}
