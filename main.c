#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblio.h"

// Main function
int main() {
    Livre book;
    int choix;
    int admin;

    FILE *fusers = fopen("users.csv", "a+");
    if (fusers == NULL) {
        printf("Error: Could not open the file :( \n");
        return 1;
    }

    printf("Are you a User or an Admin? 0:User, 1:Admin: ");
    scanf("%d", &admin);

    // If admin, ask for admin code
    if (admin == 1) {
        admin_login();
    }
    saisie_utilisateur(&book, fusers);
    fclose(fusers);

    while (1) {
        if (admin) {
            // Admin menu
            printf("Admin Menu:\nPlease choose an option:\n");
            printf("0: Ajouter un livre\n1. Modify a book\n2. Delete a book\n3. Recherche\n4. Affichage\n5. Tri\n6. Exit\n");
            scanf("%d", &choix);

            //Admin menu
            switch(choix) {
                case 0: saisieLIVRE(&book, fusers); break;
                case 1: modifierLIVRE(&book, fusers); break;
                case 2: supprimerLIVRE(&book, fusers); break;
                case 3: recherche(fusers); break;
                case 4: affichage(fusers); break;
                case 5: Tri(&book, fusers); break;
                case 6: printf("Exiting Admin Menu...Goodbye!\n"); return 0;
                default:printf("Invalid option :( Please try again!!\n"); break;
            }
        } else {
            // User menu
            printf("\nUser Menu:\nPlease choose an option:\n");
            printf("1. Recherche\n2. Affichage\n3. Tri\n4. Exit\n");
            scanf("%d", &choix);

            switch(choix) {
                case 1: recherche(fusers); break;
                case 2: affichage(fusers); break;
                case 3: Tri(&book,fusers); break;
                case 4: printf("Exiting User Menu...Goodbye;\n"); return 0;
                default: printf("Invalid choice :( Please try again!!\n"); break;
            }
        }
    }

    return 0;
}
