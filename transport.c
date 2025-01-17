#include "../include/transport.h"
#include "../include/itineraires.h"
#include "transport.h"
#include "itineraires.h"

void Ajouter_Ligne() {

    Lg Lignes;
    int nbr_arret, i;
    FILE *trajet = fopen("Lignes_gestion.csv", "a+");
    if (trajet == NULL) {
        printf("Erreur lors de l'ouverture du fichier de gestion des lignes !\n");
        exit(1);
    }

    // Lecture du nom de la ligne

    printf("Entrez le nom de la nouvelle ligne : ");
    while (getchar() != '\n');  // Vide le tampon pour éviter les résidus
    fgets(Lignes.Nom_ligne, sizeof(Lignes.Nom_ligne), stdin);
    Lignes.Nom_ligne[strcspn(Lignes.Nom_ligne, "\n")] = '\0'; // Supprime le '\n' ajouté par fgets

    printf("\nEntrez le nombre d'arrets pour cette ligne : ");
    scanf("%d", &nbr_arret);
    getchar(); // Consomme le retour à la ligne après `scanf`

    // Ajout du nom de la ligne dans le fichier
    fprintf(trajet, "Ligne : %s\n", Lignes.Nom_ligne);

    for (i = 0; i < nbr_arret; i++) {
        // Lecture du nom de l'arrêt
        printf("Entrez le nom de l'arret %d : ", i + 1);
        fgets(Lignes.arret[i].Nom_arret, sizeof(Lignes.arret[i].Nom_arret), stdin);
        Lignes.arret[i].Nom_arret[strcspn(Lignes.arret[i].Nom_arret, "\n")] = '\0'; // Supprime le '\n'

        // Lecture de l'heure de passage
        printf("Entrez l'heure de passage : ");
        fgets(Lignes.arret[i].heure_passage, sizeof(Lignes.arret[i].heure_passage), stdin);
        Lignes.arret[i].heure_passage[strcspn(Lignes.arret[i].heure_passage, "\n")] = '\0'; // Supprime le '\n'

        // Écriture dans le fichier
        fprintf(trajet, "Arret %d: %s, Heure: %s\n", i + 1, Lignes.arret[i].Nom_arret, Lignes.arret[i].heure_passage);
    }

    fclose(trajet);
    printf("Ligne ajoutee avec succes !\n");
}

 
void Affichage_Lignes() {
    int ligne_choix;  // Variable pour stocker le choix de l'utilisateur

    // Ouvrir le fichier contenant les lignes en mode lecture
    FILE *trajet = fopen("Lignes_gestion.csv", "r");
    if (trajet == NULL) {
        printf("Erreur dans l'ouverture du fichier de gestion des lignes\n");
        exit(1);  // Quitter le programme si le fichier ne peut pas être ouvert
    }

    // Demander à l'utilisateur s'il veut afficher toutes les lignes ou une ligne spécifique
    do {
        printf("Voulez-vous afficher toutes les lignes (1) ou bien une ligne specifiee (2) ? Entrez 1 ou 2 :\n");
        scanf("%d", &ligne_choix);
    } while (ligne_choix != 1 && ligne_choix != 2);  // S'assurer que l'utilisateur entre une valeur valide (1 ou 2)

    if (ligne_choix == 1) {
        // Bloc pour afficher toutes les lignes
        char ligne_txt[max];  // Buffer pour la lecture des lignes du fichier

        // Lire le fichier ligne par ligne
        while (fgets(ligne_txt, sizeof(ligne_txt), trajet)) {
            // Vérifier si la ligne contient un nom de ligne
            if (strncmp(ligne_txt, "Ligne :", 7) == 0) {
                printf("\n%s", ligne_txt);  // Afficher le nom de la ligne
            } else {
                printf("%s", ligne_txt);  // Afficher les arrêts et leurs heures associés
            }
        }

    } else {
        // Bloc pour afficher une ligne spécifique
        char nv_ligne[max];  // Nom de la ligne à rechercher
        char ligne_txt[max];  // Buffer pour la lecture du fichier
        int ligne_trouvee = 0;  // Indicateur pour vérifier si la ligne a été trouvée

        // Demander à l'utilisateur de saisir le nom de la ligne à rechercher
        printf("Entrez le nom de la ligne a afficher : ");
        getchar();  // Consommer le retour à la ligne laissé par scanf
        fgets(nv_ligne, sizeof(nv_ligne), stdin);  // Lire le nom de la ligne
        nv_ligne[strcspn(nv_ligne, "\n")] = '\0';  // Supprimer le retour à la ligne ajouté par fgets

        // Repositionner le pointeur au début du fichier pour une nouvelle lecture
        rewind(trajet);

        // Parcourir le fichier pour trouver la ligne recherchée
        while (fgets(ligne_txt, sizeof(ligne_txt), trajet)) {
            // Vérifier si la ligne contient un nom de ligne
            if (strncmp(ligne_txt, "Ligne :", 7) == 0) {
                char ligne_nom[max];  // Variable pour stocker le nom de la ligne extraite
                sscanf(ligne_txt, "Ligne : %[^\n]", ligne_nom);  // Extraire le nom de la ligne

                // Vérifier si le nom extrait correspond à celui recherché
                if (strcmp(ligne_nom, nv_ligne) == 0) {
                    ligne_trouvee = 1;  // Marquer la ligne comme trouvée
                    printf("\n%s\n", ligne_txt);  // Afficher le nom de la ligne

                    // Lire et afficher les arrêts associés
                    while (fgets(ligne_txt, sizeof(ligne_txt), trajet) && strncmp(ligne_txt, "Ligne :", 7) != 0) {
                        printf("%s", ligne_txt);  // Afficher chaque arrêt
                    }
                    break;  // Arrêter la recherche après avoir trouvé et affiché la ligne
                }
            }
        }

        // Si la ligne n'a pas été trouvée, afficher un message d'erreur
        if (!ligne_trouvee) {
            printf("La ligne %s n'a pas ete trouvee.\n", nv_ligne);
        }
    }

    // Fermer le fichier après utilisation
    fclose(trajet);
}
 
  
void Modifier_Ligne() {
    FILE *trajet = fopen("Lignes_gestion.csv", "r");
    if (trajet == NULL) {
        printf("Erreur dans l'ouverture du fichier.\n");
        exit(1);
    }

    Lg Lignes;
    char line_mod[max];    // Nom de la ligne à modifier
    char ligne_txt[max];   // Ligne lue depuis le fichier
    char new_nom[max];     // Nouveau nom de la ligne
    int new_nbr;           // Nouveau nombre d'arrêts
    char new_arret[max];   
    char new_heure[max];  
    int ligne_trouvee = 0; 

    // Demander le nom de la ligne à modifier
    printf("Entrez le nom de la ligne a modifier : ");
    getchar(); // Consommer tout caractère restant dans le tampon
    fgets(line_mod, sizeof(line_mod), stdin);
    line_mod[strcspn(line_mod, "\n")] = '\0'; // Supprimer le '\n'

    // Créer un fichier temporaire
    FILE *temp_file = fopen("temp.csv", "w");
    if (temp_file == NULL) {
        printf("Erreur dans l'ouverture du fichier temporaire.\n");
        fclose(trajet);
        exit(1);
    }

    // Lire le fichier ligne par ligne
    while (fgets(ligne_txt, sizeof(ligne_txt), trajet)) {
        // Extraire le nom de la ligne et le nombre d'arrêts
        sscanf(ligne_txt, "%[^,],%d", Lignes.Nom_ligne, &Lignes.nb_arrets);

        if (strcmp(Lignes.Nom_ligne, line_mod) == 0) {
            ligne_trouvee = 1;

            // Demander les nouvelles informations
            printf("Entrez le nouveau nom de la ligne : ");
            fgets(new_nom, sizeof(new_nom), stdin);
            new_nom[strcspn(new_nom, "\n")] = '\0';

            printf("Entrez le nouveau nombre d'arrets : ");
            scanf("%d", &new_nbr);
            getchar(); // Consommer le retour à la ligne restant

            // Ecrire le nouveau nom et le nombre d'arrêts dans le fichier temporaire
            fprintf(temp_file, "%s,%d\n", new_nom, new_nbr);

            // Demander les nouveaux détails des arrêts
            for (int i = 0; i < new_nbr; i++) {
                printf("Entrez le nom du nouvel arret %d : ", i + 1);
                fgets(new_arret, sizeof(new_arret), stdin);
                new_arret[strcspn(new_arret, "\n")] = '\0';

                printf("Entrez la nouvelle heure de passage pour cet arret : ");
                fgets(new_heure, sizeof(new_heure), stdin);
                new_heure[strcspn(new_heure, "\n")] = '\0';

                // Ecrire l'arrêt et son heure de passage dans le fichier temporaire
                fprintf(temp_file, "%s,%s\n", new_arret, new_heure);
            }
        } else {
            // Copier les données non modifiées dans le fichier temporaire
            fputs(ligne_txt, temp_file);

            for (int i = 0; i < Lignes.nb_arrets; i++) {
                if (fgets(ligne_txt, sizeof(ligne_txt), trajet)) {
                    fputs(ligne_txt, temp_file);
                }
            }
        }
    }

    fclose(trajet);
    fclose(temp_file);

    // Remplacer le fichier d'origine par le fichier temporaire
    if (ligne_trouvee) {
        remove("Lignes_gestion.csv");
        rename("temp.csv", "Lignes_gestion.csv");
        printf("La ligne a ete modifiee avec succes.\n");
    } else {
        remove("temp.csv");
        printf("La ligne specifiee n'a pas ete trouvee.\n");
    }
}


   
   void Supprimer_Ligne() {
    FILE *trajet = fopen("Lignes_gestion.csv", "r");
    if (trajet == NULL) {
        printf("Erreur dans l'ouverture du fichier de gestion des lignes\n");
        exit(1);  
    }

    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Erreur lors de la creation du fichier temporaire.\n");
        fclose(trajet);
        exit(1); 
    }

    Lg Lignes;
    char Ligne_rmv[max];
    char ligne_txt[max];
    int ligne_trouvee = 0;
    int ignorer_ligne = 0;

    // Saisie du nom de la ligne à supprimer
    printf("Entrez le nom de la ligne a supprimer : ");
    scanf(" %[^\n]", Ligne_rmv);

    // Lecture du fichier ligne par ligne
    while (fgets(ligne_txt, sizeof(ligne_txt), trajet)) {
        // Vérifier si c'est une ligne ou un arrêt
        if (strncmp(ligne_txt, "Ligne :", 7) == 0) {
            // Extraire le nom de la ligne
            sscanf(ligne_txt, "Ligne : %[^\n]", Lignes.Nom_ligne);
            printf("Nom de ligne lu : '%s'\n", Lignes.Nom_ligne);

            // Vérifier si cette ligne correspond à celle à supprimer
            if (strcmp(Lignes.Nom_ligne, Ligne_rmv) == 0) {
                ligne_trouvee = 1;
                ignorer_ligne = 1; // Activer le mode suppression
                continue;          // Ignorer cette ligne
            }
        }

        // Ignorer les arrêts associés à la ligne trouvée
        if (ignorer_ligne) {
            if (strncmp(ligne_txt, "Arret", 5) == 0) {
                continue; // Ignorer les arrêts
            } else {
                ignorer_ligne = 0; // Fin des arrêts
            }
        }

        // Copier les autres lignes dans le fichier temporaire
        fputs(ligne_txt,temp);
    }

    fclose(temp);
    fclose(trajet);

    // Remplacer le fichier original par le fichier temporaire
    if (ligne_trouvee) {
        remove("Lignes_gestion.csv");           // Supprimer l'ancien fichier
        rename("temp.csv", "Lignes_gestion.csv");  // Renommer le fichier temporaire
        printf("La ligne '%s' a ete supprimee avec succes.\n", Ligne_rmv);
    } else {
        remove("temp.csv");  // Nettoyer le fichier temporaire
        printf("La ligne '%s' n'a pas ete trouvee.\n", Ligne_rmv);
    }
}
