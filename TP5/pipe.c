#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int tube[2];
    pid_t pid;
    if (pipe(tube) == -1) {
        perror("erreur pipe");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        perror("erreur fork");
        exit(1);
    }
    if (pid == 0) {
        // on ferme le côté lecture
        close(tube[0]);
        // initialisation du générateur aléatoire
        srand(getpid());
        while (1) {
            // attente aléatoire entre 1 et 5 secondes
            int attente = (rand() % 5) + 1;
            sleep(attente);
            // génération d'un chiffre
            int nombre = rand() % 100;
            // écriture dans le tube
            printf("fils : j'envoie %d après %d secondes\n", nombre, attente);
            write(tube[1], &nombre, sizeof(int));
        }
    } else {
        close(tube[1]);
        // on rend le côté lecture du tube non bloquant
        int flags = fcntl(tube[0], F_GETFL, 0);
        fcntl(tube[0], F_SETFL, flags | O_NONBLOCK);
        int buffer;
        while (1) {
            // le père vérifie toutes les secondes
            sleep(1);
            // tentative de lecture
            int n = read(tube[0], &buffer, sizeof(int));

            if (n > 0) {
                printf("père : reçu le nombre %d\n", buffer);
            } else if (n == -1) {
                // si erreur est eagain, le tube est vide temporairement
                if (errno == EAGAIN) {
                    printf("père : rien de nouveau dans le tube...\n");
                } else {
                    perror("erreur de lecture");
                    exit(1);
                }
            } else {
                // n == 0, le fils a fermé le tube (fin du processus)
                printf("le fils a terminé.\n");
                break;
            }
        }
    }
    return 0;
}