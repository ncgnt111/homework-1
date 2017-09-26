#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "digits.h"

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf(" The program %s must have only one argument!\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strlen(argv[1]) > PATH_MAX) {
        printf("Length of an argument is more than PATH_MAX!\n");
        return EXIT_FAILURE;
    }

    int result_modify = 0;

    result_modify = modify_file(argv[1]);

    if (!result_modify) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
