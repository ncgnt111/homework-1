#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int add_number_to_array(int **array, size_t *size_array, int number) {
    int idx = 0;

    if (number == 0) {
        idx = 0;
    } else if (number < 0) {
        idx = 1;
    } else {
        idx = 2;
    }

    int *addr = NULL;
    ++(size_array[idx]);

    addr = (int*)realloc(array[idx], (size_array[idx]) * sizeof(int)); // change the size of the memory block

    if (addr != NULL) {
    array[idx] = addr;
    array[idx][size_array[idx]-1] = number; // write a number to the corresponding array
    return EXIT_SUCCESS;
    } else {
        printf("ERROR: %s\n", strerror(errno));

        for(int i = 0; i < 3; i++)
            free(array[i]); // realloc doesn't free it in case of fail
        free(array);

        return EXIT_FAILURE;
    }
}

int write_array_to_file(FILE *fp, int **array, size_t *size_array) {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < size_array[i]; ++j) {
            fprintf(fp, "%d ", array[i][j]);

            if (errno) {
                printf("ERROR: %s\n", strerror(errno));

                for(int i = 0; i < 3; i++)
                    free(array[i]); // realloc doesn't free it in case of fail
                free(array);

                fclose(fp);
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

int modify_file(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf("ERROR: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Create multidimensional dynamic array for zeros, negative and positive values
    int **values = NULL;
    values = malloc(3 * sizeof(int *));
    if(values == NULL) {
        printf("ERROR: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    for(int i = 0; i < 3; i++) {
        values[i] = NULL;
    }

    size_t size_values[3] = {0};

    int number = 0;

    int result_fscanf = 0;
    int result_add_number = 0;
    int result_write_array = 0;

    while (((result_fscanf = fscanf(fp, "%d", &number)) != EOF)) {
        if (result_fscanf == 0) {
            printf("File must contain only integer values!\n");
            for(int i = 0; i < 3; i++)
                free(values[i]);
            free(values);
            fclose(fp);
            return EXIT_FAILURE;
        } else {
            result_add_number = add_number_to_array(values, size_values, number);
            if (result_add_number == EXIT_FAILURE) {
                fclose(fp);
                return EXIT_FAILURE;
            }
        }
    }

    fclose(fp);
    fp = fopen(file_name, "w");
    if (!fp) {
        printf("ERROR: %s\n", strerror(errno));
        for(int i = 0; i < 3; i++)
            free(values[i]);
        free(values);
        return EXIT_FAILURE;
    }

    // Write values from multidimensional dynamic array to the txt-file:
    // zeros, then negative and positive values
    result_write_array = write_array_to_file(fp, values, size_values);

    if (result_write_array == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    for(int i = 0; i < 3; i++)
        free(values[i]);
    free(values);

    fclose(fp);

    return EXIT_SUCCESS;
}
