#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

#define DELAI 5
volatile int compteur = 0;

// fonction gestionnaire qui sera appelée à chaque alarme
void alarme_handler(int sig) {
    // on utilise \n pour forcer l'affichage immédiat
    printf("\n[alarme] nombre de mots lus ces %d dernières secondes : %d\n", DELAI, compteur);
    compteur = 0;
    alarm(DELAI);
}

int main() {
    signal(SIGALRM, alarme_handler);
    alarm(DELAI);
    printf("tapez du texte (le compteur s'affiche toutes les %d sec). ctrl+c pour quitter.\n", DELAI);
    char c;
    int dans_un_mot = 0; // etat : 0 = hors mot, 1 = dans un mot
    while (read(0, &c, 1) > 0) {
        // vérification si le caractère est un espace (blanc, tab, retour ligne)
        if (isspace(c)) {
            dans_un_mot = 0;
        } 
        // si ce n'est pas un espace et qu'on n'était pas déjà dans un mot
        else if (dans_un_mot == 0) {
            dans_un_mot = 1;
            compteur++;
        }
    }
    return 0;
}