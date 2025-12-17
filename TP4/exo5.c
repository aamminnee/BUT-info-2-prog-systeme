#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile unsigned long byte_count = 0;

void alrm_handler(int signum) {
    printf("Vitesse : %lu o/s\n", byte_count);
    byte_count = 0;
    alarm(1);
}

int main() {
    char buf[1024];
    ssize_t n;

    signal(SIGALRM, alrm_handler);
    alarm(1);

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        byte_count += n;
    }

    return 0;
}