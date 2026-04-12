#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // The rename system call changes the name or location of a file
    if (rename(argv[1], argv[2]) != 0) {
        perror("custom_mv: Error moving file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}