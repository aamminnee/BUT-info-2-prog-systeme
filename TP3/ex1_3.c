#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>  

int main() {
    printf("Bonjour\n");
    pid_t pid = fork();
    if (pid == 0) {
        printf("Je suis le fils\n");
    } else {
        printf("Je suis le père\n");
    }
    return 0;
}
