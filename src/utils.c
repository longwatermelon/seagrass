#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* utils_read_file(const char* path)
{
    FILE* file = fopen(path, "r");

    if (!file)
    {
        fprintf(stderr, "Couldn't open file %s\n", path);
        return 0;
    }

    char* contents = malloc(sizeof(char));
    contents[0] = '\0';

    char* line = 0;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
    {
        int prev_len = strlen(contents);
        contents = realloc(contents, sizeof(char) * (prev_len + read + 1));

        memcpy(&contents[prev_len], line, read);
        contents[prev_len + read] = '\0';
    }

    free(line);
    fclose(file);

    return contents;
}

