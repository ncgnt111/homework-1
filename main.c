#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "digits.h"

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf(" The command %s must have only one argument\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strlen(argv[1]) > PATH_MAX) {
        printf("Length of an argument is more than PATH_MAX.\n");
        return EXIT_FAILURE;
    }

    modify_file(argv[1]);

    return EXIT_SUCCESS;
}
