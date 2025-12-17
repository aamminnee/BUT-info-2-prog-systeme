#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <sys/types.h>

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
        close(tube[0]);
        srand(getpid());
        while (1) {
            int attente = (rand() % 5) + 1;
            sleep(attente);
            int nombre = rand() % 100;
            printf("fils : j'envoie %d après %d secondes\n", nombre, attente);
            write(tube[1], &nombre, sizeof(int));
        }
    } else {
        close(tube[1]);        
        int buffer;
        fd_set ensemble_lecture;
        struct timeval delai;
        while (1) {
            // réinitialisation de l'ensemble des descripteurs
            FD_ZERO(&ensemble_lecture);
            // ajout du descripteur du tube à surveiller
            FD_SET(tube[0], &ensemble_lecture);

            // configuration du délai d'attente (timeout) à 1 seconde
            // important : on doit le refaire à chaque tour de boucle
            // car select peut modifier cette structure sur linux
            delai.tv_sec = 1;
            delai.tv_usec = 0;
            int ret = select(tube[0] + 1, &ensemble_lecture, NULL, NULL, &delai);
            if (ret == -1) {
                perror("erreur select");
                exit(1);
            } else if (ret == 0) {
                // le délai s'est écoulé sans données
                printf("père : rien de nouveau (timeout select)\n");
            } else {
                // des données sont disponibles, on peut lire sans bloquer
                if (FD_ISSET(tube[0], &ensemble_lecture)) {
                    int n = read(tube[0], &buffer, sizeof(int));
                    if (n > 0) {
                        printf("père : reçu le nombre %d\n", buffer);
                    } else {
                        printf("le fils a terminé.\n");
                        break;
                    }
                }
            }
        }
    }
    return 0;
}