#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // vérification du nombre d'arguments
    if (argc != 2) {
        fprintf(stderr, "usage: %s <nombre_entre_1_et_100>\n", argv[0]);
        exit(1);
    }
    // conversion de l'argument en entier
    int n = atoi(argv[1]);
    // vérification que l'entier est compris entre 1 et 100
    if (n < 1 || n > 100) {
        fprintf(stderr, "erreur: le nombre doit etre compris entre 1 et 100.\n");
        exit(1);
    }
    // création des processus
    for (int i = 1; i <= n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            // gestion de l'erreur de fork
            perror("erreur lors du fork");
            exit(1);
        } else if (pid == 0) {
            // code du processus fils
            printf("Je suis le fils numéro %d de père %d\n", i, getppid());
            sleep(5);
            printf("processus fils %d terminé\n", getpid());
            // le fils se termine
            exit(0);
        }
    }
    // le père attend la fin de tous ses fils
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    return 0;
}