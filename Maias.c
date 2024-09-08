#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compareStrings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void sortStrings(const char* arr[], int n) {
    qsort(arr, n, sizeof(const char*), compareStrings);
}

int removeDuplicates(const char* arr[], int n) {
    if (n == 0 || n == 1)
        return n;

    char **temp = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        temp[i] = malloc(20 * sizeof(char));
    }

    int j = 0;
    for (int i = 0; i < n - 1; i++) {
        if (strcmp(arr[i], arr[i + 1]) != 0) {
            strcpy(temp[j++], arr[i]);
        }
    }
    strcpy(temp[j++], arr[n - 1]);

    for (int i = 0; i < j; i++) {
        strcpy((char*)arr[i], temp[i]);
    }

    for (int i = 0; i < n; i++) {
        free(temp[i]);
    }
    free(temp);

    return j;
}

int main() {
    char **arr1 = malloc(216856 * sizeof(char*));
    char **arr2 = malloc(216856 * sizeof(char*));
    char **arr3 = malloc(216856 * sizeof(char*));
    int *wordFrequency = malloc(216856 * sizeof(int));

    for (int i = 0; i < 216856; i++) {
        arr1[i] = malloc(20 * sizeof(char));
        arr2[i] = malloc(20 * sizeof(char));
        arr3[i] = malloc(20 * sizeof(char));
    }

    char currentWord[20], nextWord[20], predictionWord[20];
    FILE *inputFile = fopen("MAIAS.txt", "r");
    FILE *outputFile = fopen("Previsao.txt", "w");

    if (inputFile == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        exit(EXIT_FAILURE);
    }

    int wordCount = 0;

    while (fscanf(inputFile, "%s", currentWord) != EOF) {
        strcpy(arr1[wordCount++], currentWord);
    }

    fclose(inputFile);

    for (int j = 0; j < wordCount; j++) {
        int cont = 1;
        strcpy(nextWord, arr1[j]);

        int k;
        for (k = j; k < wordCount - 1; k++) {
            if (strcmp(nextWord, arr1[k + 1]) == 0) {
                strcpy(arr2[cont++], arr1[k + 2]);
            }
        }

        sortStrings(arr2, cont); 
        memcpy(arr3, arr2, cont * sizeof(char*));
        int uniqueCount = removeDuplicates(arr3, cont);

        // Contar frequencias e output
        for (int m = 0; m < uniqueCount; m++) {
            int frequency = 0;
            for (int s = 0; s < cont; s++) {
                if (strcmp(arr3[m], arr2[s]) == 0)
                    frequency++;
            }
            wordFrequency[m] = frequency;

            int previousOccurrence = 0;
            for (int g = 0; g < j; g++) {
                if (strcmp(nextWord, arr1[g]) == 0) {
                    previousOccurrence++;
                }
            }

            if (previousOccurrence == 0) {
                fprintf(outputFile, "%s %s %d\n", nextWord, arr3[m], wordFrequency[m]);
            }
        }
    }

    fclose(outputFile);

    // Input utilizador e predicao sugestoes
    while (1) {
        printf("\nEnter a word: ");
        fgets(currentWord, sizeof(currentWord), stdin);
        currentWord[strcspn(currentWord, "\n")] = 0;

        printf("\nSuggestions:\n");

        inputFile = fopen("Previsao.txt", "r");

        if (inputFile == NULL) {
            fprintf(stderr, "Error opening prediction file.\n");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (fscanf(inputFile, "%s", predictionWord) != EOF) {
            if (strcmp(predictionWord, currentWord) == 0) {
                fscanf(inputFile, "%s", nextWord);
                fscanf(inputFile, "%d", &wordFrequency[i]);
                strcpy(arr2[i++], nextWord);
            }
        }

        fclose(inputFile);

        int maxFrequency = 0;
        for (int j = 0; j < i; j++) {
            if (wordFrequency[j] > maxFrequency) {
                maxFrequency = wordFrequency[j];
            }
        }

        int displayed = 0;
        for (int currentMax = maxFrequency; currentMax > 0 && displayed < 3; currentMax--) {
            for (int j = 0; j < i; j++) {
                if (wordFrequency[j] == currentMax) {
                    printf("%s\n", arr2[j]);
                    if (++displayed == 3) break;
                }
            }
        }
    }

    for (int i = 0; i < 216856; i++) {
        free(arr1[i]);
        free(arr2[i]);
        free(arr3[i]);
    }

    free(arr1);
    free(arr2);
    free(arr3);
    free(wordFrequency);

    return 0;
}
