#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int x = 5;
    pid_t pid = fork();
    if (pid == 0) {
        printf("FILS -> valeur de x = %d, adresse = %p\n", x, (void*)&x);
    } else {
        printf("PERE -> valeur de x = %d, adresse = %p\n", x, (void*)&x);
    }
    return 0;
}
