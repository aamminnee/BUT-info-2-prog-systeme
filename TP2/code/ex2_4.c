#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>   
#include <stdlib.h>  

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s fichier1 [fichier2 ...]\n", argv[0]);
        return 1;
    }
    char buffer[512];
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror(argv[i]);
            continue;  
        }
        ssize_t n;
        while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
            if (write(1, buffer, n) == -1) {
                perror("Erreur write");
                close(fd);
                return 1;
            }
        }
        if (n == -1) {
            perror("Erreur read");
        }
        close(fd);
    }
    return 0;
}
