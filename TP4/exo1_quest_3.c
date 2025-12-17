#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    write(STDOUT_FILENO, "Salut!", 6);
    fork();
    fork();
    sleep(5);
    write(STDOUT_FILENO, "\n", 1);
    return 0;
}