#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // For opendir(), readdir(), closedir()

int main(int argc, char *argv[]) {
    // Default to the current directory if no path is provided
    const char *path = (argc == 1) ? "." : argv[1];

    // Open the directory stream
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("custom_ls: Cannot open directory");
        return 1;
    }

    struct dirent *entry;

    // Read the directory entries one by one
    while ((entry = readdir(dir)) != NULL) {
        // Standard ls behavior: ignore hidden files (those starting with '.')
        if (entry->d_name[0] != '.') {
            printf("%s  ", entry->d_name);
        }
    }
    printf("\n"); // Print a final newline for clean formatting

    closedir(dir);
    return 0;
}