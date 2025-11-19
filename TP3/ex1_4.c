#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>  

int main() {
    printf("Bonjour\n");
    pid_t pid = fork();
    if (pid == 0) {
        printf("je suis le fils, PID = %d\n", getpid());
    } else {
        printf("je suis le pere, PID = %d\n", getpid());
    }
    return 0;
}
