#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

