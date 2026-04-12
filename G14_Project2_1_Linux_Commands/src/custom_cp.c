#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    // We need exactly a source and a destination
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open source file for reading
    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0) {
        perror("custom_cp: Error opening source file");
        return EXIT_FAILURE;
    }

    // Open destination file for writing (create if it doesn't exist, truncate if it does)
    // 0644 sets standard file permissions (rw-r--r--)
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("custom_cp: Error opening/creating destination file");
        close(fd_in);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Read chunks from source and write to destination
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        if (write(fd_out, buffer, bytes_read) != bytes_read) {
            perror("custom_cp: Error writing to destination file");
            close(fd_in);
            close(fd_out);
            return EXIT_FAILURE;
        }
    }

    if (bytes_read < 0) {
        perror("custom_cp: Error reading source file");
    }

    close(fd_in);
    close(fd_out);
    return EXIT_SUCCESS;
}