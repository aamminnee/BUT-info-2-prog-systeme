#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signum) {
    printf("\nIl ne peut en rester qu'un !\n");
}

int main() {
    if (signal(SIGINT, handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    while (1) {
        pause();
    }
    return 0;
}