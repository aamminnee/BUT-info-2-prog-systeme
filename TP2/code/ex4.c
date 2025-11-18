#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


void archiver(const char* nomArchive, const char* nomExe) {
    DIR* dir = opendir(".");
    if (!dir) {
        perror("open répertoire");
        exit(1);
    }
    int fdArchive = open(nomArchive, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (fdArchive == -1) {
        perror("open de l’archive");
        closedir(dir);
        exit(1);
    }
    struct dirent* entry;
    char buffer[512];
    while ((entry = readdir(dir)) != NULL) {
        char* nomFichier = entry->d_name;
        if (strcmp(nomFichier, ".") == 0 || strcmp(nomFichier, "..") == 0 ||
            strcmp(nomFichier, nomArchive) == 0 || strcmp(nomFichier, nomExe) == 0)
            continue;
        struct stat st;
        if (stat(nomFichier, &st) == -1) {
            perror(nomFichier);
            continue;
        }
        off_t tailleFichier = st.st_size;
        size_t tailleNom = strlen(nomFichier);
        char temp[20];
        int n = snprintf(temp, sizeof(temp), "%zu", tailleNom);
        if (write(fdArchive, temp, n) != n) {
            perror("write taille nom");
            close(fdArchive);
            closedir(dir);
            exit(1);
        }
        if (write(fdArchive, nomFichier, tailleNom) != tailleNom) {
            perror("write nomFichier");
            close(fdArchive);
            closedir(dir);
            exit(1);
        }
        n = snprintf(temp, sizeof(temp), "%lld", (long long)tailleFichier);
        if (write(fdArchive, temp, n) != n) {
            perror("write taille fichier");
            close(fdArchive);
            closedir(dir);
            exit(1);
        }
        int fdF = open(nomFichier, O_RDONLY);
        if (fdF == -1) {
            perror(nomFichier);
            continue;
        }
        ssize_t lu;
        while ((lu = read(fdF, buffer, sizeof(buffer))) > 0) {
            if (write(fdArchive, buffer, lu) != lu) {
                perror("write contenu fichier");
                close(fdF);
                close(fdArchive);
                closedir(dir);
                exit(1);
            }
        }
        if (lu == -1) perror("read fichier");
        close(fdF);
    }
    close(fdArchive);
    closedir(dir);
    printf("Archivage terminé dans %s\n", nomArchive);
}


void extraire(const char* nomArchive) {
    int fdArchive = open(nomArchive, O_RDONLY);
    if (fdArchive == -1) {
        perror("open archive");
        exit(1);
    }
    char buffer[512];
    while (1) {
        char tailleNomStr[20];
        int i = 0;
        char c;
        while (read(fdArchive, &c, 1) == 1 && c >= '0' && c <= '9') {
            tailleNomStr[i++] = c;
        }
        if (i == 0) break; 
        tailleNomStr[i] = '\0';
        size_t tailleNom = atoi(tailleNomStr);
        char nomFichier[256];
        if (read(fdArchive, nomFichier, tailleNom) != tailleNom) {
            perror("read nomFichier");
            close(fdArchive);
            exit(1);
        }
        nomFichier[tailleNom] = '\0';
        char tailleFichierStr[20];
        i = 0;
        while (read(fdArchive, &c, 1) == 1 && c >= '0' && c <= '9') {
            tailleFichierStr[i++] = c;
        }
        tailleFichierStr[i] = '\0';
        off_t tailleFichier = atoll(tailleFichierStr);
        int fdF = open(nomFichier, O_WRONLY | O_CREAT | O_TRUNC, 0664);
        if (fdF == -1) {
            perror(nomFichier);
            close(fdArchive);
            exit(1);
        }
        off_t restant = tailleFichier;
        while (restant > 0) {
            ssize_t n = read(fdArchive, buffer, (restant > sizeof(buffer)) ? sizeof(buffer) : restant);
            if (n <= 0) {
                perror("read contenu fichier");
                close(fdF);
                close(fdArchive);
                exit(1);
            }
            if (write(fdF, buffer, n) != n) {
                perror("write contenu fichier");
                close(fdF);
                close(fdArchive);
                exit(1);
            }
            restant -= n;
        }
        close(fdF);
    }
    close(fdArchive);
    printf("Extraction terminée depuis %s\n", nomArchive);
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage : %s archive [archiver|extraire]\n", argv[0]);
        return 1;
    }
    const char* nomArchive = argv[1];
    const char* action = argv[2];
    const char* nomExe = argv[0]; 
    if (strcmp(action, "archiver") == 0) {
        archiver(nomArchive, nomExe);
    } else if (strcmp(action, "extraire") == 0) {
        extraire(nomArchive);
    } else {
        fprintf(stderr, "action inconnue : %s (utiliser archiver ou extraire)\n", action);
        return 1;
    }
    return 0;
}
