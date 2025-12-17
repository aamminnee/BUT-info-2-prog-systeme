#include <unistd.h>
#include <stdio.h>

int main() {
    printf("Salut!\n");
    fork();
    fork();
    sleep(5);
    printf("\n");
    return 0;
}