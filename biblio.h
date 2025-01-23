#ifndef BIBLIO_H
#define BIBLIO_H

// Define the structure Livre
typedef struct {
    char ID[20];
    char titre[100];
    char auteur[50];
    char description[200];
    char NomUti[50];
    float prix;
    char categorie[50];
    int disponibilite;  // 1 for available, 0 for not available
} Livre;

// Function declarations
int is_name_taken(FILE *f, char *NomUti);
void saisie_utilisateur(Livre *book, FILE *f);
int admin_login();
void saisieLIVRE(Livre *book, FILE *f);
void modifierLIVRE(Livre *book, FILE *f);
void supprimerLIVRE(Livre *book, FILE *f);
void affichage(FILE *f);
void recherche(FILE *f);
void Tri(Livre book[], FILE *f);

#endif  // BIBLIO_H
