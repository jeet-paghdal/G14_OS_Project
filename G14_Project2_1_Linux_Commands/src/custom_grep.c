#define _POSIX_C_SOURCE 200809L

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
    int ignore_case;
    int line_number;
    int invert_match;
    int count_only;
    int list_files;
} GrepOptions;

typedef struct {
    char *data;
    size_t length;
} LineBuffer;

static void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [OPTION]... PATTERN [FILE]...\n", program_name);
    fprintf(stderr, "Search for PATTERN in each FILE or standard input.\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "  -i  ignore case distinctions\n");
    fprintf(stderr, "  -n  print line number with output lines\n");
    fprintf(stderr, "  -v  select non-matching lines\n");
    fprintf(stderr, "  -c  print only a count of matching lines\n");
    fprintf(stderr, "  -l  print only names of FILEs with matches\n");
}

static void free_line(LineBuffer *buffer) {
    free(buffer->data);
    buffer->data = NULL;
    buffer->length = 0;
}

static ssize_t read_line(FILE *stream, LineBuffer *buffer) {
    int ch;
    size_t capacity = 0;

    free_line(buffer);

    while ((ch = fgetc(stream)) != EOF) {
        if (buffer->length + 1 >= capacity) {
            size_t new_capacity = (capacity == 0) ? 128 : capacity * 2;
            char *new_data = realloc(buffer->data, new_capacity);
            if (new_data == NULL) {
                free_line(buffer);
                return -1;
            }
            buffer->data = new_data;
            capacity = new_capacity;
        }

        buffer->data[buffer->length++] = (char)ch;
        if (ch == '\n') {
            break;
        }
    }

    if (ch == EOF && buffer->length == 0) {
        return -1;
    }

    if (buffer->length + 1 >= capacity) {
        char *new_data = realloc(buffer->data, buffer->length + 1);
        if (new_data == NULL) {
            free_line(buffer);
            return -1;
        }
        buffer->data = new_data;
    }

    buffer->data[buffer->length] = '\0';
    return (ssize_t)buffer->length;
}

static int matches_pattern(const regex_t *regex, const char *line) {
    return regexec(regex, line, 0, NULL, 0) == 0;
}

static int process_stream(FILE *stream, const char *filename, const regex_t *regex,
                          const GrepOptions *options, int multiple_files) {
    LineBuffer buffer = {NULL, 0};
    ssize_t line_length;
    long line_number = 0;
    long match_count = 0;

    while ((line_length = read_line(stream, &buffer)) >= 0) {
        int matched;

        (void)line_length;
        line_number++;
        matched = matches_pattern(regex, buffer.data);
        if (options->invert_match) {
            matched = !matched;
        }

        if (matched) {
            match_count++;

            if (options->list_files) {
                printf("%s\n", filename);
                free_line(&buffer);
                return 0;
            }
        }

        if (matched && !options->count_only && !options->list_files) {
            if (multiple_files && filename != NULL) {
                printf("%s:", filename);
            }
            if (options->line_number) {
                printf("%ld:", line_number);
            }
            fputs(buffer.data, stdout);
            if (buffer.length == 0 || buffer.data[buffer.length - 1] != '\n') {
                putchar('\n');
            }
        }
    }

    if (options->count_only) {
        if (multiple_files && filename != NULL) {
            printf("%s:", filename);
        }
        printf("%ld\n", match_count);
    }

    free_line(&buffer);
    return 0;
}

int main(int argc, char *argv[]) {
    GrepOptions options = {0, 0, 0, 0, 0};
    char *pattern = NULL;
    int first_file_index = 0;
    int regex_flags = REG_EXTENDED;
    int i;

    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        }

        if (argv[i][0] != '-' || argv[i][1] == '\0' || strcmp(argv[i], "-") == 0) {
            pattern = argv[i];
            first_file_index = i + 1;
            break;
        }

        if (argv[i][1] == '-') {
            fprintf(stderr, "custom_grep: unsupported option '%s'\n", argv[i]);
            return EXIT_FAILURE;
        }

        for (size_t j = 1; argv[i][j] != '\0'; j++) {
            switch (argv[i][j]) {
                case 'i':
                    options.ignore_case = 1;
                    break;
                case 'n':
                    options.line_number = 1;
                    break;
                case 'v':
                    options.invert_match = 1;
                    break;
                case 'c':
                    options.count_only = 1;
                    break;
                case 'l':
                    options.list_files = 1;
                    break;
                default:
                    fprintf(stderr, "custom_grep: invalid option -- '%c'\n", argv[i][j]);
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
            }
        }
    }

    if (pattern == NULL) {
        fprintf(stderr, "custom_grep: missing pattern\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (options.ignore_case) {
        regex_flags |= REG_ICASE;
    }

    regex_t regex;
    if (regcomp(&regex, pattern, regex_flags) != 0) {
        fprintf(stderr, "custom_grep: invalid pattern: %s\n", pattern);
        return EXIT_FAILURE;
    }

    if (first_file_index >= argc) {
        int status = process_stream(stdin, NULL, &regex, &options, 0);
        regfree(&regex);
        return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    {
        int file_count = argc - first_file_index;
        int multiple_files = (file_count > 1);
        int status = EXIT_SUCCESS;

        for (i = first_file_index; i < argc; i++) {
            FILE *stream = fopen(argv[i], "r");
            if (stream == NULL) {
                perror("custom_grep");
                status = EXIT_FAILURE;
                continue;
            }

            if (process_stream(stream, argv[i], &regex, &options, multiple_files) != 0) {
                status = EXIT_FAILURE;
            }

            fclose(stream);
        }

        regfree(&regex);
        return status;
    }
}