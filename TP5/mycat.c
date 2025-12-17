#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // vérification qu'un fichier est passé en argument
    if (argc != 2) {
        char *msg = "usage: ./main <fichier>\n";
        write(2, msg, strlen(msg));
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    // gestion de l'erreur d'ouverture
    if (fd == -1) {
        perror("erreur lors de l'ouverture du fichier");
        exit(1);
    }
    char c;
    int debut_de_ligne = 1; // 1 si on est au début d'une ligne, 0 sinon
    // lecture du fichier caractère par caractère
    while (read(fd, &c, 1) > 0) {
        if (debut_de_ligne && c == '\n') {
            char *etoiles = "**********\n";
            write(1, etoiles, 11);
            // on reste en début de ligne pour la prochaine itération
            debut_de_ligne = 1; 
        } else {
            // sinon on affiche le caractère normalement
            write(1, &c, 1);
            // mise à jour du statut : si on vient d'écrire \n, on est au début d'une nouvelle ligne
            if (c == '\n') {
                debut_de_ligne = 1;
            } else {
                debut_de_ligne = 0;
            }
        }
    }
    if (close(fd) == -1) {
        perror("erreur lors de la fermeture du fichier");
        exit(1);
    }
    return 0;
}