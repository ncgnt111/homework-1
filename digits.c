#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void add_number_to_array(int **array, size_t *size_array, int *ptr_number) {
    int *addr = NULL;
    ++(*size_array);
    addr = (int*)realloc(*array, (*size_array) * sizeof(int)); // change the size of the memory block

    if (addr != NULL) {
        *array = addr;
        (*array)[(*size_array) - 1] = *ptr_number; // write a number to the corresponding array
    } else {
        free (*array); // realloc doesn't free it in case of fail
        printf("ERROR: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void write_array_to_file(FILE *fp, int *array, size_t *size_array) {
    for (size_t i = 0; i < *size_array; ++i) {
        if(!(fprintf(fp, "%d ", array[i]))) {
            printf("ERROR: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

void modify_file(char *file_name) {
    FILE *fp = fopen(file_name, "r");

    if (!fp) {
        printf("ERROR: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Create three dynamic arrays for zeros, negative and positive values
    int *zer_values = NULL;
    int *neg_values = NULL;
    int *pos_values = NULL;

    size_t size_zer_values = 0;
    size_t size_neg_values = 0;
    size_t size_pos_values = 0;

    int number = 0;

    int result = 0;

    while (((result = fscanf(fp, "%d", &number)) != EOF)) {
        if (result == 0) {
            printf("File must contain only integer values\n");
            exit(EXIT_FAILURE);
        } else {
            if (number == 0) {
                add_number_to_array(&zer_values, &size_zer_values, &number);
            } else if (number < 0) {
                add_number_to_array(&neg_values, &size_neg_values, &number);
            } else {
                add_number_to_array(&pos_values, &size_pos_values, &number);
            }
        }
    }

    fclose(fp);

    fp = fopen(file_name, "w");

    if (!fp) {
        printf("ERROR: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Write values from dynamic arrays to the txt-file:
    // zeros, then negative and positive values
    write_array_to_file(fp, zer_values, &size_zer_values);
    write_array_to_file(fp, neg_values, &size_neg_values);
    write_array_to_file(fp, pos_values, &size_pos_values);

    free(zer_values);
    free(neg_values);
    free(pos_values);

    fclose(fp);
}

