#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int add_number_to_array(int **array, int *used,int *size, int number) {
    int idx = 0;

    if (number == 0) {
        idx = 0;
    } else if (number < 0) {
        idx = 1;
    } else {
        idx = 2;
    }

    ++(used[idx]);

    if (used[idx] > size[idx]) {

        int *addr = NULL;

        size[idx] *= 2;

        addr = (int*)realloc(array[idx], (size[idx]) * sizeof(int)); // change the size of the memory block
        if (addr != NULL) {
            array[idx] = addr;
        } else {
            printf("ERROR: %s\n", strerror(errno));

            for(int i = 0; i < 3; i++)
                free(array[i]); // realloc doesn't free it in case of fail
            free(array);

            return EXIT_FAILURE;
        }
    }

    array[idx][used[idx] - 1] = number; // write a number to the corresponding array
    return EXIT_SUCCESS;
}

int write_array_to_file(FILE *fp, int **array, int *used) {
    int result_fprintf = 0;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < used[i]; ++j) {

            result_fprintf = fprintf(fp, "%d ", array[i][j]);

            if (result_fprintf < 0) {
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
    FILE *fp = NULL;
    fp = fopen(file_name, "r");
    if (!fp) {
        printf("ERROR: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Create multidimensional dynamic array for zeros, negative and positive values

    int used[3] = {0, 0, 0};
    int size[3] = {100, 100, 100};

    int **values = NULL;
    values = malloc(3 * sizeof(int *));
    if(values == NULL) {
        printf("ERROR: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    for(int i = 0; i < 3; i++) {
        values[i] = malloc(size[i] * sizeof(int));
        if (values[i] == NULL) {
            printf("ERROR: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
    }

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
            result_add_number = add_number_to_array(values, used, size, number);
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
    result_write_array = write_array_to_file(fp, values, used);
    if (result_write_array == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    for(int i = 0; i < 3; i++)
        free(values[i]);
    free(values);

    fclose(fp);

    return EXIT_SUCCESS;
}
