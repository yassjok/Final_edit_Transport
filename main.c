#include "../include/transport.h"
#include "../include/itineraires.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 800

int main() {
    int choix;

    do {
        // Affichage du menu principal
        printf("\033[4;34m_____ Gestion des lignes de transport _____\033[0m\n"); // Couleur bleue
        printf("1. Ajouter une ligne\n");
        printf("2. Affichage des lignes disponibles\n");
        printf("3. Modifier une ligne\n");
        printf("4. Supprimer une ligne\n");
        printf("5. Rechercher un itineraire sans correspondances\n");
        printf("6. Recherche avancee d'un itineraire avec correspondances\n");
        printf("7. Quitter le programme\n");
        printf("Entrez votre choix : ");
        
        if (scanf("%d", &choix) != 1) { // Vérifie une entrée valide
            printf("Veuillez entrer un nombre valide.\n");
            while (getchar() != '\n'); // Vider le tampon en cas d'entrée invalide
            continue;
        }

        getchar(); // Consomme le retour à la ligne laissé par scanf

        switch (choix) {
            case 1:
                Ajouter_Ligne(); // Ajouter une ligne
                break;
            case 2:
                Affichage_Lignes(); // Afficher les lignes
                break;
            case 3:
                Modifier_Ligne(); // Modifier une ligne
                break;
            case 4:
                Supprimer_Ligne(); // Supprimer une ligne
                break;
            case 5:
                rechercher_itineraire_meme_ligne(); // Rechercher un itinéraire sans correspondances
                break;
            case 6:
                rechercher_itineraire_correspondances(); // Rechercher un itinéraire avec correspondances
                break;
            case 7:
                printf("Au revoir !\n"); // Quitter le programme
                break;
            default:
                printf("Choix invalide. Veuillez essayer a nouveau.\n");
                break;
        }

    } while (choix != 7); // Répéter tant que l'utilisateur ne choisit pas de quitter

    return 0;
}
