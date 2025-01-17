#ifndef transport_H
#define transport_H
#define max 800

#include "transport.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

    typedef struct {
        char Nom_arret[max];
        char heure_passage[max];
    } Arr;  // structure pour les arrets

    typedef struct {
char Nom_ligne[max];
Arr arret [max]; // une lignes peut avoir plusiurs arrets 
 int nb_arrets;         
    } Lg; // structures pour les lignes 

    void Ajouter_Ligne();
    void Affichage_Lignes();
    void Modifier_Ligne();
    void Supprimer_Ligne();

#endif