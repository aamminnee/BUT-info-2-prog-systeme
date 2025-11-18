#include <unistd.h>
#include <string.h>
#include <stdio.h>    

int main() {
    char buffer[512] = "Hello world!\n";
    //int fd = 3;  // write échouera
    int fd = 1;  // sortie standard
    if (write(fd, buffer, strlen(buffer)) == -1) {
        perror("Erreur lors de l'écriture");
        return 1;  // code d'erreur
    }

    return 0; // succès
}
