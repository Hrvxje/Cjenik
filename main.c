#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funk.h"

static int numDrinks = 0;

int main() {
    Drink* drinks = malloc(MAX_DRINKS * sizeof(Drink));
    if (drinks == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return 1;
    }

    int choice;
    do {
        printf("\nIzbornik:\n");
        printf("1. Dodaj pice\n");
        printf("2. Izbrisi pice\n");
        printf("3. Ispisi pica\n");
        printf("4. Spremi u datoteku\n");
        printf("5. Sortiraj pica\n");
        printf("6. Prikazi sadrzaj datoteke\n");
        printf("7. Izbrisi datoteku\n");
        printf("8. Preimenuj datoteku\n");
        printf("0. Izlaz\n");
        printf(">> ");

        if (scanf("%d", &choice) != 1) {
            printf("\nNeispravan unos.\n");
            while (getchar() != '\n'); // Clear input buffer
            choice = -1; // Set to invalid choice to avoid accidental valid action
        }

        switch (choice) {
        case 1:
            addDrink(drinks, &numDrinks);
            break;
        case 2:
            deleteDrink(drinks, &numDrinks);
            break;
        case 3:
            if (numDrinks == 0) {
                printf("\nNema pica na cjeniku.\n");
            }
            else {
                printDrinks(drinks, numDrinks);
            }
            break;
        case 4: {
            if (numDrinks == 0) {
                printf("\nNema dostupnih pica za spremanje u datoteku.\n");
            }
            else {
                char filename[MAX_NAME_LENGTH];
                printf("\nUnesite ime datoteke za spremanje: ");
                scanf("%s", filename);
                saveToFile(drinks, numDrinks, filename);
            }
            break;
        }
        case 5:
            sortDrinks(drinks, numDrinks);
            break;
        case 6: {
            char filename[MAX_NAME_LENGTH];
            printf("\nUnesite ime datoteke za prikaz sadrzaja: ");
            scanf("%s", filename);
            displayFileContent(filename);
            break;
        }
        case 7: {
            char filename[MAX_NAME_LENGTH];
            printf("\nUnesite ime datoteke za brisanje: ");
            scanf("%s", filename);
            deleteFile(filename);
            break;
        }
        case 8: {
            char oldName[MAX_NAME_LENGTH], newName[MAX_NAME_LENGTH];
            printf("\nUnesite staro ime datoteke: ");
            scanf("%s", oldName);
            printf("\nUnesite novo ime datoteke: ");
            scanf("%s", newName);
            renameFile(oldName, newName);
            break;
        }
        case 0:
            printf("\nIzlaz iz programa.\n");
            break;
        default:
            printf("\nNeispravan odabir.\n");
            break;
        }
    } while (choice != 0);

    safeFree((void**)&drinks);

    return 0;
}
