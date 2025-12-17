#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

int main() {
    DIR *repertoire = opendir(".");
    // vérification de l'ouverture
    if (repertoire == NULL) {
        perror("erreur ouverture repertoire");
        exit(1);
    }
    struct dirent *entree;
    int compteur = 0;
    // premier parcours pour compter les fichiers
    while ((entree = readdir(repertoire)) != NULL) {
        compteur++;
    }
    closedir(repertoire);
    int tentative;
    printf("devinez combien de fichiers sont ici : ");
    if (scanf("%d", &tentative) != 1) {
        fprintf(stderr, "erreur de saisie\n");
        exit(1);
    }
    if (tentative == compteur) {
        printf("gagné ! voici les fichiers :\n");
        // on réouvre le répertoire pour recommencer la lecture au début
        repertoire = opendir(".");
        if (repertoire == NULL) {
            perror("erreur réouverture repertoire");
            exit(1);
        }
        // second parcours pour afficher les noms
        while ((entree = readdir(repertoire)) != NULL) {
            printf("%s\n", entree->d_name);
        }
        closedir(repertoire);
    } else {
        printf("perdu ! il y a %d fichiers dans ce répertoire.\n", compteur);
    }
    return 0;
}