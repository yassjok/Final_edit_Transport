#include "../include/itineraires.h"
#include "../include/transport.h"




// Variables globales
Lg lignes[max];         // Tableau des lignes
int nb_lignes; // Appeler la fonction ;      // Nombre de lignes



void rechercher_itineraire_meme_ligne() {
    FILE *trajet = fopen("Lignes_gestion.csv", "r");
    if (trajet == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier \"gestion_Lignes.csv\"\n");
        exit(1);
    }

    FILE *etat = fopen("Etat.csv", "a");
    if (etat == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier \"Etat.csv\"\n");
        fclose(trajet);
        exit(1);
    }

    Lg ligne_actuelle ;
    char depart[max], arrivee[max];
    printf("Entrer l'arret de depart : ");
    scanf("%s", depart);
    printf("Entrer l'arret d'arrivee : ");
    scanf("%s", arrivee);

    ligne_actuelle.nb_arrets = 0; // Initialiser le nombre d'arrêts à 0
    char buffer[1024];

    // Lire le fichier ligne par ligne
    while (fgets(buffer, sizeof(buffer), trajet)) {
        // Si la ligne commence par "Ligne :", c'est une nouvelle ligne de transport
        if (strncmp(buffer, "Ligne :", 7) == 0) {
            // Vérifier si une ligne précédente contient les arrêts recherchés
            if (ligne_actuelle.nb_arrets > 0) {
                int pos_depart = -1, pos_arrivee = -1;

                // Chercher les positions des arrêts de départ et d'arrivée
                for (int i = 0; i < ligne_actuelle.nb_arrets; i++) {
                    if (strcmp(ligne_actuelle.arret[i].Nom_arret, depart) == 0) {
                        pos_depart = i;
                    }
                    if (strcmp(ligne_actuelle.arret[i].Nom_arret, arrivee) == 0) {
                        pos_arrivee = i;
                    }
                }

                // Vérifier si les deux arrêts sont trouvés et dans le bon ordre
                if (pos_depart != -1 && pos_arrivee != -1 && pos_depart < pos_arrivee) {
                    printf("Itineraire trouve sur la ligne %s :\n", ligne_actuelle.Nom_ligne);
                    fprintf(etat, "Itineraire trouve sur la ligne %s :\n", ligne_actuelle.Nom_ligne);
                    for (int i = pos_depart; i <= pos_arrivee; i++) {
                        printf("  Arret : %s, Heure : %s\n",
                               ligne_actuelle.arret[i].Nom_arret, ligne_actuelle.arret[i].heure_passage);
                        fprintf(etat, "  Arret : %s, Heure : %s\n",
                                ligne_actuelle.arret[i].Nom_arret, ligne_actuelle.arret[i].heure_passage);
                    }
                    fclose(trajet);
                    fclose(etat);
                    return; // Arrêter la recherche après avoir trouvé l'itinéraire
                }
            }

            // Nouvelle ligne de transport, réinitialiser la structure
            sscanf(buffer, "Ligne : %s", ligne_actuelle.Nom_ligne);
            ligne_actuelle.nb_arrets = 0;
        } else if (strncmp(buffer, "Arret", 5) == 0) {
            // Lire un arrêt et son heure
            char nom_arret[max], heure_passage[max];
            if (sscanf(buffer, "Arret %*d: %[^,], Heure: %s", nom_arret, heure_passage) == 2) {
                // Ajouter l'arrêt à la ligne actuelle
                strcpy(ligne_actuelle.arret[ligne_actuelle.nb_arrets].Nom_arret, nom_arret);
                strcpy(ligne_actuelle.arret[ligne_actuelle.nb_arrets].heure_passage, heure_passage);
                ligne_actuelle.nb_arrets++;
            }
        }
    }

    // Vérifier une dernière fois après la fin du fichier
    int pos_depart = -1, pos_arrivee = -1;
    for (int i = 0; i < ligne_actuelle.nb_arrets; i++) {
        if (strcmp(ligne_actuelle.arret[i].Nom_arret, depart) == 0) {
            pos_depart = i;
        }
        if (strcmp(ligne_actuelle.arret[i].Nom_arret, arrivee) == 0) {
            pos_arrivee = i;
        }
    }

    if (pos_depart != -1 && pos_arrivee != -1 && pos_depart < pos_arrivee) {
        printf("Itineraire trouve sur la ligne %s :\n", ligne_actuelle.Nom_ligne);
        fprintf(etat, "Itineraire trouve sur la ligne %s :\n", ligne_actuelle.Nom_ligne);
        for (int i = pos_depart; i <= pos_arrivee; i++) {
            printf("  Arret : %s, Heure : %s\n",
                   ligne_actuelle.arret[i].Nom_arret, ligne_actuelle.arret[i].heure_passage);
            fprintf(etat, "  Arret : %s, Heure : %s\n",
                    ligne_actuelle.arret[i].Nom_arret, ligne_actuelle.arret[i].heure_passage);
        }
    } else {
        printf("Aucun itineraire trouve entre %s et %s.\n", depart, arrivee);
        fprintf(etat, "Aucun itineraire trouve entre %s et %s.\n", depart, arrivee);
    }

    fclose(trajet);
    fclose(etat);
}


void rechercher_itineraire_correspondances() {
    char depart[max], arrivee[max];

    // Demande à l'utilisateur de saisir les arrêts de départ et d'arrivée
    printf("Entrer l'arret de depart : ");
    scanf("%s", depart);
    printf("Entrer l'arret d'arrivee : ");
    scanf("%s", arrivee);

    // Vérifie si les arrêts existent dans le réseau
    int arret_depart_trouve = 0, arret_arrivee_trouve = 0;
    for (int i = 0; i < nb_lignes; i++) {
        for (int j = 0; j < lignes[i].nb_arrets; j++) {
            if (strcmp(lignes[i].arret[j].Nom_arret, depart) == 0) arret_depart_trouve = 1;
            if (strcmp(lignes[i].arret[j].Nom_arret, arrivee) == 0) arret_arrivee_trouve = 1;
        }
    }
    if (arret_depart_trouve || arret_arrivee_trouve) {
        printf("L'un des arrets n'existe pas dans le reseau.\n");
        return;
    }

    // Recherche d'un itinéraire avec correspondance
    for (int i = 0; i <2 ; i++) {
        int pos_depart = -1;

        // Recherche de l'arrêt de départ sur Ligne_1
        for (int j = 0; j < lignes[i].nb_arrets; j++) {
            if (strcmp(lignes[i].arret[j].Nom_arret, depart) == 0) {
                pos_depart = j;
                break;
            }
        }

        if (pos_depart != -1) { // Si le départ est trouvé
            // Parcourt des autres lignes pour trouver une correspondance et l'arrivée
            for (int k = 0; k < 2; k++) {
                if (i == k) continue; // Ignore la même ligne

                int pos_inter = -1, pos_arrivee = -1;

                // Recherche d'une correspondance sur Ligne_2
                for (int m = 0; m < lignes[k].nb_arrets; m++) {
                    for (int n = 0; n < lignes[i].nb_arrets; n++) {
                        if (strcmp(lignes[i].arret[n].Nom_arret, lignes[k].arret[m].Nom_arret) == 0) {
                           pos_inter = m;
                            break;
                        }
                    }
                    if (strcmp(lignes[k].arret[m].Nom_arret, arrivee) == 0) {
                        pos_arrivee = m;
                    }
                }

                // Vérifie si une correspondance et l'arrivée sont valides
                if (pos_inter != -1 && pos_arrivee != -1 && pos_inter < pos_arrivee) {
                    printf("Itineraire trouve avec correspondance :\n");
                    printf("Ligne %s : %s -> %s\n", lignes[i].Nom_ligne, depart, lignes[i].arret[pos_inter].Nom_arret);
                    printf("Ligne %s : %s -> %s\n", lignes[k].Nom_ligne, lignes[k].arret[pos_inter].Nom_arret, arrivee);

                    // Sauvegarde de l'itinéraire dans le fichier Etat.csv
                    FILE *trajet = fopen("Etat.csv", "a");
                    if (trajet) {
                        fprintf(trajet, "Itinéraire avec correspondance : Ligne %s (%s -> %s), Ligne %s (%s -> %s)\n",
                                lignes[i].Nom_ligne, depart, lignes[i].arret[pos_inter].Nom_arret,
                                lignes[k].Nom_ligne, lignes[k].arret[pos_inter].Nom_arret, arrivee);
                        fclose(trajet);
                    }
                    return; // Arrête la recherche après avoir trouvé un itinéraire
 }
            }
        }
    }
    printf("Aucun itineraire avec correspondance trouve entre %s et %s.\n", depart,arrivee);
    }

    // Si aucun itinéraire avec correspondance n'a été trouvé


 