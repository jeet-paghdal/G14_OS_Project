#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For read(), write(), close()
#include <fcntl.h>   // For open(), O_RDONLY

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    // Check if the user provided a filename
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Loop through all provided files (e.g., custom_cat file1.txt file2.txt)
    for (int i = 1; i < argc; i++) {
        // Open the file in Read-Only mode
        int fd = open(argv[i], O_RDONLY);
        
        if (fd < 0) {
            perror("custom_cat: Error opening file");
            continue; // Skip to the next file if this one fails
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        // Read chunks of the file into the buffer and write them to standard output
        while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
            if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
                perror("custom_cat: Error writing to stdout");
                close(fd);
                return 1;
            }
        }

        if (bytes_read < 0) {
            perror("custom_cat: Error reading file");
        }

        close(fd);
    }

    return 0;
}