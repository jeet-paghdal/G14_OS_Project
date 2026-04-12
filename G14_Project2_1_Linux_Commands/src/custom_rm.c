#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PATH 1024

// Forward declaration for recursive deletion
int remove_recursive(const char *path);

// Check if a path is a directory
int is_directory(const char *path) {
    struct stat stats;
    if (stat(path, &stats) < 0) {
        return 0;
    }
    return S_ISDIR(stats.st_mode);
}

// Recursively remove a directory and its contents
int remove_recursive(const char *path) {
    DIR *dir;
    struct dirent *entry;
    char filepath[MAX_PATH];

    // Open the directory
    dir = opendir(path);
    if (dir == NULL) {
        perror("custom_rm: Error opening directory");
        return -1;
    }

    // Read and remove all entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip '.' and '..'
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full path
        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);

        // Recursively remove if it's a directory, otherwise unlink
        if (is_directory(filepath)) {
            if (remove_recursive(filepath) < 0) {
                closedir(dir);
                return -1;
            }
        } else {
            if (unlink(filepath) < 0) {
                perror("custom_rm: Error removing file");
                closedir(dir);
                return -1;
            }
        }
    }

    closedir(dir);

    // Remove the now-empty directory
    if (rmdir(path) < 0) {
        perror("custom_rm: Error removing directory");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int recursive = 0;
    int file_count = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;
        } else {
            file_count++;
        }
    }

    // Check if at least one file/directory is provided
    if (file_count == 0) {
        fprintf(stderr, "Usage: %s [-r] <file|directory> [file|directory ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Process each file/directory
    for (int i = 1; i < argc; i++) {
        // Skip the -r flag
        if (strcmp(argv[i], "-r") == 0) {
            continue;
        }

        const char *target = argv[i];

        // Check if target is a directory
        if (is_directory(target)) {
            if (!recursive) {
                fprintf(stderr, "custom_rm: Cannot remove '%s': Is a directory (use -r flag)\n", target);
                continue;
            }

            // Remove directory recursively
            if (remove_recursive(target) < 0) {
                fprintf(stderr, "custom_rm: Failed to remove directory '%s'\n", target);
            }
        } else {
            // Remove file
            if (unlink(target) < 0) {
                perror("custom_rm: Error removing file");
            }
        }
    }

    return EXIT_SUCCESS;
}
