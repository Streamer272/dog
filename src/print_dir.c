#include "print_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "print_file.h"
#include "def/alloc.h"
#include "def/style.h"

void print_path(char *path, FILE_OPTIONS file_options, DIR_OPTIONS dir_options, bool content) {
    if (path[0] == '\\')
        path++;

    FILE *file = NULL;
    struct stat stats;

    if (strcmp(path, "-") == 0)
        file = stdin;

    if (file == NULL && stat(path, &stats) == -1) {
        char *message = colored("Couldn't open %s\n", RED);
        fprintf(stderr, message, path);
        free(message);
        return;
    }

    print_file_name(path, stats, "", 0, file_options);

    if (!content)
        return;

    // check if it is a file
    if ((stats.st_mode & S_IFDIR) == 0) {
        if (file == NULL && (file = fopen(path, "r")) == NULL) {
            char *message = colored("Couldn't open %s\n", RED);
            fprintf(stderr, message, path);
            free(message);
            return;
        }

        print_file(file, file_options);

        if (file != stdin)
            fclose(file);
    } else {
        print_dir("", path, 0, file_options, dir_options);
    }
}

void print_dir(char *start, char *path, int nested_depth, FILE_OPTIONS file_options, DIR_OPTIONS dir_options) {
    char *current_path = NULL;
    if (strcmp(start, "") == 0 || strcmp(start, ".") == 0 || strncmp(start, path, strlen(start)) == 0) {
        current_path = alloc(strlen(path) + 1);
        strcpy(current_path, path);
    }
    else {
        current_path = alloc(strlen(start) + 1 + strlen(path) + 1);
        sprintf("%s/%s", start, path);
    }

    char *original_current_path = current_path;
    long recursion_depth = 3;
    if (strcmp(dir_options.recursion_depth, "") != 0)
        recursion_depth = strtol(dir_options.recursion_depth, NULL, 10);

    DIR *dir;
    struct dirent *entry;

    if (current_path[0] == '\\')
        current_path++;

    if ((dir = opendir(current_path)) == NULL) {
        char *message = colored("Couldn't open %s\n", RED);
        fprintf(stderr, message, current_path);
        free(message);
        free(current_path);
        return;
    }

    // TODO: sort entries by name
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (entry->d_name[0] == '.' && !dir_options.show_hidden)
            continue;

        char *file_name = alloc(strlen(current_path) + 1 + strlen(entry->d_name) + 1);
        sprintf(file_name, "%s/%s", current_path, entry->d_name);

        struct stat stats;
        if (stat(file_name, &stats) == -1) {
            char *message = colored("Couldn't open %s\n", RED);
            fprintf(stderr, message, file_name);
            free(message);
            free(file_name);
            continue;
        }

        char *prefix = alloc((nested_depth + 1) * 4 + 1);
        for (int i = 0; i < nested_depth + 1; i++) {
            strcat(prefix, "    ");
        }
        print_file_name(file_name, stats, prefix, 0, file_options);
        free(prefix);

        if ((stats.st_mode & S_IFDIR) != 0 && dir_options.recursive && nested_depth + 1 < recursion_depth) {
            print_dir(current_path, file_name, nested_depth + 1, file_options, dir_options);
            printf("\n");
        }

        free(file_name);
    }

    closedir(dir);
    free(original_current_path);
}
