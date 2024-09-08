#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "nova_palavra.h"

static Elem_palavras head = NULL;

int total_words = 0;
int unique_words = 0;

Elem_palavras create_word(char *word) {
    Elem_palavras new_node = (Elem_palavras)malloc(sizeof(struct Palavras));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    new_node->name = (char *)malloc((strlen(word) + 1) * sizeof(char));
    if (new_node->name == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(new_node->name, word);
    new_node->occurrences = 1;
    new_node->next = NULL;
    unique_words++;
    return new_node;
}

void insert_word(char *word) {
    Elem_palavras new_node = create_word(word);
    if (head == NULL) {
        head = new_node;
    } else {
        Elem_palavras temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int check_word(char *word) {
    Elem_palavras temp = head;

    while (temp != NULL) {
        // For case-insensitive comparison, use strcasecmp
        if (strcmp(temp->name, word) == 0) {
            temp->occurrences++;
            return temp->occurrences;
        }
        temp = temp->next;
    }

    insert_word(word);
    return 1;
}

void create_word_list_from_file(char *filename) {
    char word[100];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fscanf(file, "%s", word) != EOF) {
        check_word(word);
        total_words++;
    }
    fclose(file);
}

void write_reverse(FILE *file, Elem_palavras node) {
    if (node->next != NULL) {
        write_reverse(file, node->next);
    }
    fprintf(file, "%d: \"%s\"\n", node->occurrences, node->name);
}

void write_word_list_to_file(char *filename) {
    char *output_filename;
    char extension[] = ".palavras";
    Elem_palavras temp = head;
    FILE *file = NULL;

    output_filename = (char *)malloc((strlen(filename) + strlen(extension) + 1) * sizeof(char));
    if (output_filename == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    strcpy(output_filename, filename);
    strcat(output_filename, extension);

    file = fopen(output_filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(1);
    }

    while (temp != NULL) {
        fprintf(file, "%d: \"%s\"\n", temp->occurrences, temp->name);
        temp = temp->next;
    }

    free(output_filename);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid arguments. Usage: ./program_name <file_name>\n");
        return 2;
    }

    char *input_filename = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    if (input_filename == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(input_filename, argv[1]);

    create_word_list_from_file(input_filename);
    write_word_list_to_file(input_filename);

    char *reverse_filename;
    char reverse_extension[] = ".end_to_head.txt";
    reverse_filename = (char *)malloc((strlen(input_filename) + strlen(reverse_extension) + 1) * sizeof(char));
    if (reverse_filename == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(reverse_filename, input_filename);
    strcat(reverse_filename, reverse_extension);

    FILE *reverse_file = fopen(reverse_filename, "w");
    if (reverse_file == NULL) {
        printf("Error opening reverse output file.\n");
        exit(1);
    }
    write_reverse(reverse_file, head);
    fclose(reverse_file);

    printf("Total words = %d, Unique words = %d\n", total_words, unique_words);

    Elem_palavras current = head;
    while (current != NULL) {
        Elem_palavras temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
    free(input_filename);
    free(reverse_filename);

    return 0;
}
