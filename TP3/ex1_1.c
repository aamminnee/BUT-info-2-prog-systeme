#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Bonjour\n");
    fork();         
    printf("Au revoir !\n");
    return 0;
}
