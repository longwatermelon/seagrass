#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


char** fs_read_dir(const char* path, unsigned char type, int* count)
{
    DIR* dir = opendir(path);

    if (!dir)
    {
        fprintf(stderr, "Couldn't open directory %s\n", path);
        return 0;
    }

    char** items = malloc(0);
    *count = 0;

    struct dirent* de;

    while ((de = readdir(dir)) != 0)
    {
        if (de->d_type == type)
        {
            if (de->d_type == DT_DIR &&
                (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0))
                continue;

            items = realloc(items, sizeof(char*) * ++*count);
            items[*count - 1] = malloc(sizeof(char) * (strlen(path) + strlen(de->d_name) + 2));

            snprintf(items[*count - 1], strlen(path) + strlen(de->d_name) + 2, "%s/%s", path, de->d_name);
        }
    }

    closedir(dir);
    return items;
}


char* fs_parent(const char* file)
{
    for (int i = strlen(file) - 1; i >= 0; --i)
    {
        if (file[i] == '/')
        {
            char* parent = malloc(sizeof(char) * (i + 1));
            snprintf(parent, i + 1, "%s", file);
            return parent;
        }
    }

    return 0;
}


char* fs_filename(const char* file)
{
    for (int i = strlen(file) - 1; i >= 0; --i)
    {
        if (file[i] == '/')
        {
            char* name = malloc(sizeof(char) * (strlen(file) - i + 1));
            snprintf(name, strlen(file) - i + 1, "%s", &file[i + 1]);
            return name;
        }
    }

    return 0;
}

