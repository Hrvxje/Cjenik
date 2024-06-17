#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "funk.h"

extern int numDrinks;

void addDrink(Drink* drinks, int* numDrinks) {
    if (drinks == NULL || numDrinks == NULL) {
        printf("\nNeispravan pokazivac.\n");
        return;
    }

    if (*numDrinks >= MAX_DRINKS) {
        printf("\nNema dovoljno memorije za unos novog pica.\n");
        return;
    }

    printf("\nUnesite ime pica: ");
    scanf("%s", drinks[*numDrinks].name);
    printf("\nUnesite cijenu pica: ");
    while (scanf("%f", &drinks[*numDrinks].price) != 1) {
        printf("Neispravan unos, unesite ponovo cijenu: ");
        while (getchar() != '\n'); // Clear input buffer
    }

    (*numDrinks)++;
}

void deleteDrink(Drink* drinks, int* numDrinks) {
    if (drinks == NULL || numDrinks == NULL) {
        printf("\nNeispravan pokazivac.\n");
        return;
    }

    if (*numDrinks == 0) {
        printf("\nNema dostupnih pica za brisanje.\n");
        return;
    }

    int index;
    printf("\nUnesite broj pica koje zelite obrisati: ");
    while (scanf("%d", &index) != 1 || index < 1 || index > *numDrinks) {
        printf("\nNeispravan unos, unesite ponovo broj pica: ");
        while (getchar() != '\n'); // Clear input buffer
    }

    for (int i = index - 1; i < *numDrinks - 1; i++) {
        strcpy(drinks[i].name, drinks[i + 1].name);
        drinks[i].price = drinks[i + 1].price;
    }

    (*numDrinks)--;
    printf("\nUspjesno izbrisano.\n");
}

void saveToFile(Drink* drinks, int numDrinks, const char* filename) {
    if (drinks == NULL || filename == NULL) {
        printf("\nNeispravan pokazivac.\n");
        return;
    }

    if (numDrinks == 0) {
        printf("\nNema dostupnih pica za spremanje u datoteku.\n");
        return;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("\nGreska pri otvaranju datoteke za pisanje: %s\n", strerror(errno));
        return;
    }

    for (int i = 0; i < numDrinks; i++) {
        fprintf(file, "%s - %.2feur\n", drinks[i].name, drinks[i].price);
    }

    if (ferror(file)) {
        printf("\nGreska pri pisanju u datoteku: %s\n", strerror(errno));
    }
    else {
        printf("\nPodaci su uspjesno spremljeni u %s\n", filename);
    }

    fclose(file);
}

void sortDrinks(Drink* drinks, int numDrinks) {
    quicksort(drinks, 0, numDrinks - 1);
    printf("\nPica su uspjesno sortirana.\n");
}

Drink* searchDrink(Drink* drinks, int numDrinks, const char* name) {
    return (Drink*)bsearch(name, drinks, numDrinks, sizeof(Drink), compareDrinks);
}

void quicksort(Drink* drinks, int low, int high) {
    if (low < high) {
        int pi = partition(drinks, low, high);
        quicksort(drinks, low, pi - 1);
        quicksort(drinks, pi + 1, high);
    }
}

int partition(Drink* drinks, int low, int high) {
    Drink pivot = drinks[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (compareDrinks(&drinks[j], &pivot) < 0) {
            i++;
            swap(&drinks[i], &drinks[j]);
        }
    }
    swap(&drinks[i + 1], &drinks[high]);
    return (i + 1);
}

void swap(Drink* a, Drink* b) {
    Drink t = *a;
    *a = *b;
    *b = t;
}

void copyFile(const char* src, const char* dest) {
    FILE* source = fopen(src, "rb");
    if (source == NULL) {
        printf("\nGreska pri otvaranju izvorne datoteke: %s\n", strerror(errno));
        return;
    }

    FILE* destination = fopen(dest, "wb");
    if (destination == NULL) {
        printf("\nGreska pri otvaranju odredisne datoteke: %s\n", strerror(errno));
        fclose(source);
        return;
    }

    char buffer[BUFSIZ];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, n, destination) != n) {
            printf("\nGreska pri pisanju u datoteku: %s\n", strerror(errno));
            fclose(source);
            fclose(destination);
            return;
        }
    }

    fclose(source);
    fclose(destination);
    printf("\nDatoteka je uspjesno kopirana.\n");
}

void deleteFile(const char* filename) {
    if (filename == NULL) {
        printf("\nNeispravan pokazivac.\n");
        return;
    }

    if (remove(filename) == 0) {
        printf("\nDatoteka %s je uspjesno obrisana.\n", filename);
    }
    else {
        printf("\nGreska pri brisanju datoteke %s: %s\n", filename, strerror(errno));
    }
}

void renameFile(const char* oldName, const char* newName) {
    if (oldName == NULL || newName == NULL) {
        printf("\nNeispravan pokazivac.\n");
        return;
    }

    if (rename(oldName, newName) == 0) {
        printf("\nDatoteka %s je uspjesno preimenovana u %s.\n", oldName, newName);
    }
    else {
        printf("\nGreska pri preimenovanju datoteke %s u %s: %s\n", oldName, newName, strerror(errno));
    }
}

void displayFileContent(const char* filename) {
    if (filename == NULL) {
        printf("\nNeispravan pokazivac.\n");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("\nGreska pri otvaranju datoteke za citanje: %s\n", strerror(errno));
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc((fileSize + 1) * sizeof(char));
    if (buffer == NULL) {
        printf("\nGreska pri alokaciji memorije.\n");
        fclose(file);
        return;
    }

    fread(buffer, sizeof(char), fileSize, file);
    buffer[fileSize] = '\0';

    printf("\nSadrzaj datoteke %s:\n%s\n", filename, buffer);

    free(buffer);
    fclose(file);
}

void safeFree(void** ptr) {
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}
