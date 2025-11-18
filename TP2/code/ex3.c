#include <stdio.h>      // fopen, fprintf, fclose
#include <stdlib.h>     // exit
#include <string.h>     // strlen
#include <dirent.h>     // opendir, readdir, closedir


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier_sortie\n", argv[0]);
        return 1;
    }
    DIR* dir = opendir(".");
    if (dir == NULL) {
        perror("Erreur ouverture répertoire");
        return 1;
    }
    FILE* sortie = fopen(argv[1], "w");
    if (sortie == NULL) {
        perror("Erreur ouverture fichier sortie");
        closedir(dir);
        return 1;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        char* nomFichier = entry->d_name;
        size_t taille = strlen(nomFichier);
        fprintf(sortie, "%zu%s", taille, nomFichier);
    }
    fclose(sortie);
    closedir(dir);
    return 0;
}
