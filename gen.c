#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(" The command %s must have only one argument\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *f = NULL;

    f = fopen(argv[1], "w");
    if (!f) {
        printf("ERROR: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 10000; ++i) {
        fprintf(f, "%d ", (-500 + (rand() % 1001)));
    }

    fclose(f);

    return EXIT_SUCCESS;
}

