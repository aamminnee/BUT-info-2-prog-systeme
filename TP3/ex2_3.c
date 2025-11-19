#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int x = 5;
    pid_t pid = fork();
    if (pid == 0) {
        x += 2;
        printf("FILS -> x = %d, adresse = %p\n", x, (void*)&x);
    } else {
        x -= 5;
        printf("PERE -> x = %d, adresse = %p\n", x, (void*)&x);
    }
    return 0;
}
