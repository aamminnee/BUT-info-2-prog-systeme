#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>  

int main() {
    printf("Bonjour\n");
    pid_t pid = fork();
    if (pid == 0) {
        printf("Fils : pid = %d, mon PID = %d\n", pid, getpid());
    } else {
        printf("Pere : pid = %d, mon PID = %d\n", pid, getpid());
    }
    return 0;
}
