#pragma once
#ifndef DRINKS_H
#define DRINKS_H

#define MAX_DRINKS 100
#define MAX_NAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    float price;
} Drink;

void addDrink(Drink* drinks, int* numDrinks);
void deleteDrink(Drink* drinks, int* numDrinks);
inline void printDrinks(Drink* drinks, int numDrinks) {
    printf("\nPopis pica:\n");
    for (int i = 0; i < numDrinks; i++) {
        printf("%d. %s - %.2feur\n", i + 1, drinks[i].name, drinks[i].price);
    }
}
void saveToFile(Drink* drinks, int numDrinks, const char* filename);
void sortDrinks(Drink* drinks, int numDrinks);
inline int compareDrinks(const void* a, const void* b) {
    Drink* drinkA = (Drink*)a;
    Drink* drinkB = (Drink*)b;
    return strcmp(drinkA->name, drinkB->name);
}
Drink* searchDrink(Drink* drinks, int numDrinks, const char* name);
void quicksort(Drink* drinks, int low, int high);
int partition(Drink* drinks, int low, int high);
void swap(Drink* a, Drink* b);
void copyFile(const char* src, const char* dest);
void deleteFile(const char* filename);
void renameFile(const char* oldName, const char* newName);
void displayFileContent(const char* filename);
void safeFree(void** ptr);

#endif
