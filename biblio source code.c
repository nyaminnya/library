#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblio.h"

// Function to check if a username is taken
int is_name_taken(FILE *f, char *NomUti) {
    rewind(f);
    char Nom[100]; //temp variable to store the name

    while (fgets(Nom, sizeof(Nom), f)) {
        Nom[strcspn(Nom, "\n")] = '\0';
        //string complementary span //returns the length of the string //removes \n

        if (strcmp(Nom, NomUti) == 0) {
            return 1;  //taken
        }
    }
    return 0;  //free
}

// Function user input (user's name and verification)
void saisie_utilisateur(Livre *book, FILE *f){
int admin;

while(1) //continuously ask the user for a unique username until one is successfully registered
    {
    printf("Enter your name: \n");
    scanf("%99s", book->NomUti); //Limit input size to avoid overflow

   if (is_name_taken(f, book->NomUti)) {
            printf("This name is already taken, please choose another one: \n");
        } else {
    printf("Welcome %s!\n", book->NomUti);
    fprintf(f, "%s\n", book->NomUti); //writes in file and adds \n after
    fflush(f); //written immediately
            break;
        }
    }
     if (admin) {
        printf("You are logged in as an Admin.\n");
    } else {
        printf("You are logged in as a User.\n");
    }
}


// Function for admin login
int admin_login() {
    // check for password (the password is admin123)
    char password[10];

    printf("Enter admin password: \n");
    scanf("%s", password);

    if (strcmp(password, "admin123") == 0) {
        printf("Admin logged in.\n");
        return 1; //admin verified
    } else {
        printf("Invalid admin password.\n");
        return 0; //not an admin
    }
}

// Function to add a new book
void saisieLIVRE(Livre *book, FILE *f) {
    printf("Enter book ID: ");
    scanf("%s", book->ID);

    printf("Enter book title: ");
    scanf("%[^\n]", book->titre);
     //reads all characters until \n is encountered. It stores in the name variable but does not include \n

    printf("Enter book author: ");
    scanf("%[^\n]", book->auteur);

    printf("Enter book description: ");
    scanf("%[^\n]", book->description);

    printf("Enter book price: ");
    scanf("%f", &book->prix);

    printf("Enter book category: ");
    scanf("%[^\n]", book->categorie);

    printf("Enter book availability (1 for available, 0 for not available): ");
    scanf("%d", &book->disponibilite);

    fprintf(f, "%s,%s,%s,%s,%s,%.2f,%s,%d\n", book->ID, book->titre, book->auteur,
            book->description, book->NomUti, book->prix, book->categorie, book->disponibilite);

    printf("Book added successfully.\n");
}

// Function to modify an existing book
void modifierLIVRE(Livre *book, FILE *f) {

char newid[50];
int found = 0,choix;

    FILE *ftemp = fopen("tempbooks.csv", "w+");

    if (ftemp == NULL) {
        printf("Error opening temporary file.\n");
        return;
    }

    printf("Enter book ID to modify: ");
    scanf("%s", newid);

    printf("Que voulez-vous modifier? 0: ID\n 1: Titre \n 2: Auteur \n 3: Description \n 4: Prix \n 5: Categorie \n 6: Dsponibilite");
    scanf("%d", &choix);

    while(fgets(book->ID, sizeof(book->ID),f)){
    if (strcmp(book->ID,newid) == 0){
        found=1;
        printf("Modifying book %s...\n", book->ID);

     switch(choix){
        case 0:
        printf("Saisir le nouveau ID: \n");
        scanf ("%s", book->ID); break;
        case 1:
        printf("Saisir le nouveau titre: \n");
        scanf(" %[^\n]", book->titre);break;
        case 2:
        printf("Saisir le nouveau auteur: \n");
        scanf(" %[^\n]", book->auteur);break;
        case 3:
        printf("Saisir la nouvelle description: \n");
        scanf(" %[^\n]", book->description);break;
        case 4:
        printf("Saisir le nouveau prix: \n");
        scanf ("%f", &book->prix); break;
        case 5:
        printf("Saisir la nouvelle categorie: \n");
        scanf ("%s", book->categorie); break;
        case 6:
        printf("Saisir le nouveau disponibilite: 0 pour non disponible et 1 pour disponible\n");
        scanf ("%d", &book->disponibilite); break;
        default: printf("error, please choose another number.\n"); break;
     }}}
        fprintf(ftemp, "%s,%s,%s,%s,%s,%.2f,%s,%d\n", book->ID, book->titre, book->auteur,
                book->description, book->NomUti, book->prix, book->categorie, book->disponibilite);

    fclose(f);
    fclose(ftemp);

if(found){
    remove("books.csv");
    rename("tempbooks.csv", "books.csv");
    printf("Book was modified successfully! Thank you for your contribution :)\n");
 }
else{
    remove("tempbooks.csv");
    printf("No book with this ID found, please enter another ID.\n");
}}

// Function to delete a book from the library
void supprimerLIVRE(Livre *book, FILE *f) {

char newid[50], line[600];
int found = 0;

FILE *ftemp = fopen("temp.csv", "w");
FILE *fdeletedbooks= fopen("deleted_books.csv", "a");

if (fdeletedbooks == NULL) {
        printf("Error\n");
        return;}

if (ftemp == NULL) {
        printf("Error opening temporary file.\n");
        fclose(fdeletedbooks);
        //handles the case where fdeletedbooks was opened successfully earlier but temp file can't be opened
        return;}

    printf("Enter book ID to delete: ");
    scanf("%s", newid);

// Loop through each line in the original file
while (fgets(line, sizeof(line), f)) { // Extract fields from the line
    sscanf(line, "%[^,],", book->ID);// Read book ID (until the first comma)
        if (strcmp(book->ID, newid) != 0) {
    // Book ID does not match, write line to temp file
            fputs(line, ftemp);
        } else {
            // Book ID matches
            fprintf(fdeletedbooks, "%s", line);
            found = 1;
        }
    }

    fclose(f);
    fclose(ftemp);
    fclose(fdeletedbooks);

    if (!found) {
        printf("Book with ID %s not found.\n", newid);
        remove("temp.csv"); // Delete the temporary file if the book was not found
    } else {
        remove("books.csv");
        rename("temp.csv", "books.csv");
        printf("Book deleted successfully.\n");
    }
}

// Function to display all books
void affichage(FILE *f) {
    char line[600];
    Livre book;

    rewind(f);

    printf("Liste des livres disponibles dans la bibliothèque:\n");

   // Lire chaque ligne du fichier
    while (fgets(line, sizeof(line), f)) {
    // Extraire les informations de chaque ligne
    sscanf(line, "%[^,],%[^,],%s[^,],%[^,],%[^,],%f,%[^,],%d",
               book.ID, book.titre, book.auteur, book.description, book.NomUti, &book.prix, book.categorie, &book.disponibilite);

     // Afficher les informations du livre
        printf("  -> ID: %s\n", book.ID);
        printf("  -> Titre: %s\n", book.titre);
        printf("  -> Auteur: %s\n", book.auteur);
        printf("  -> Prix: %s\n", book.prix);
        printf("  -> Description: %s\n", book.description);
        printf("  -> Utilisateur: %s\n", book.NomUti);
        printf("  -> Catégorie: %s\n", book.categorie);
        printf("  -> Disponibilité: %s\n\n", book.disponibilite ? 1 : 0);
    }
}

// Function to search books by title or author
void recherche(FILE *f) {

Livre book;
int choice, found=0;
char newtitre[100], newauteur[50], line[600];

printf("Veuillez-vous cherchez un livre par titre: 0 ou auteur: 1? \n");
scanf("%d", &choice);

rewind(f);

switch (choice){
    case 0:
    printf("Enter le titre: \n");
    fgets(newtitre, sizeof(newtitre), stdin); // Read the title
            newtitre[strcspn(newtitre, "\n")] = '\0'; // Remove \n

    while (fgets(line, sizeof(line), f)) {
                // Lire chaque champ jusqu'à un saut de ligne

    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%f,%[^,],%d",
            book.ID, book.titre, book.auteur, book.description,
            &book.prix, book.categorie, &book.disponibilite);

    if (strstr(book.titre, newtitre) != NULL || strstr(book.auteur, newauteur) != NULL) {
            printf("Livre trouvé.\n");
            printf("ID: %s\nTitle: %s\nAuthor: %s\nDescription: %s\nCategory: %s\nPrice: %.2f\nAvailability: %d\n\n",
                   book.ID, book.titre, book.auteur, book.description, book.categorie, book.prix, book.disponibilite);
            found = 1;
        }
    }

    if (!found) {
        printf("No books found.\n");
    } break;

    case 1:
    printf("Entrez l'auteur: \n");
    fgets(newauteur, sizeof(newauteur), stdin); // Lire l'auteur
        newauteur[strcspn(newauteur, "\n")] = '\0'; // Supprimer le \n

    while (fgets(line, sizeof(line), f)) {
    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%f,%[^,],%d",
        book.ID, book.titre, book.auteur, book.description,
        &book.prix, book.categorie, &book.disponibilite);

    // Vérifier si l'auteur correspond
    if (strstr(book.auteur, newauteur) != NULL) {
    printf("Livre trouvé:\n");
    printf("ID: %s\nTitre: %s\nAuteur: %s\nDescription: %s\nPrix: %.2f\nCatégorie: %s\nDisponibilité: %s\n\n",
        book.ID, book.titre, book.auteur, book.description, book.prix,
        book.categorie, book.disponibilite ? 1 : 0);
                    found = 1;
                }
            }
            if (found == 0) {
                printf("Aucun livre trouvé avec cet auteur.\n");
            }
            break;
        default:
            printf("Erreur. Veuillez entrer 0 ou 1.\n");
            break;
    }
}

//trier les livres
void Tri(Livre book[], FILE *f){

Livre temp;
int choix, i, j, cmpt=0, result;

while (fscanf(f, "%[^,],%[^,],%[^,],%[^,],%f,%[^,],%d\n",
        book[cmpt].ID, book[cmpt].titre, book[cmpt].auteur, book[cmpt].description,
        &book[cmpt].prix, book[cmpt].categorie, &book[cmpt].disponibilite) != EOF) {
        cmpt++;  // Increment the counter after each successful read
    }
fclose(f);

printf("Do you want to tri par Titre: 0 ou Categorie: 1?");
scanf("%d", &choix);

switch(choix){
case 0:
for (i = 0; i < cmpt - 1; i++){
    for (j= i + 1; j < cmpt; j++){
    result = strcmp(book[i].titre, book[j].titre);
        if (result > 0) { //i>j
temp = book[i];
book[i]=book[j];
book[j]= temp;
        }
    }
}
break;

case 1:
for (i = 0; i < cmpt - 1; i++){
    for (j = i + 1; j < cmpt; j++){
    result = strcmp(book[i].categorie, book[j].categorie);
        if (result >0) { //i>j
temp = book[i];
book[i]=book[j];
book[j]= temp;
        }
    }
}
break;

 default: printf("Invalid choice. Please enter 0 for Titre or 1 for Categorie.\n");return;

}
// AFFICHAGE
    for (i = 0; i < cmpt; i++) {
        printf("ID: %s, Titre: %s, Auteur: %s, Description: %s, Prix: %.2f, Categorie: %s, Disponibilité: %s\n",
               book[i].ID, book[i].titre, book[i].auteur, book[i].description, book[i].prix,
               book[i].categorie, book[i].disponibilite ? 1 : 0);

    }
}
