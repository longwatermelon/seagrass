#include "tree.h"
#include "../utils.h"
#include <dirent.h>
#include <SDL_ttf.h>


struct Tree* tree_alloc(SDL_Point pos, const char* path, SDL_Renderer* rend)
{
    struct Tree* self = malloc(sizeof(struct Tree));
    self->path = path;
    self->pos = pos;
    self->path = path;

    self->font = TTF_OpenFont("res/font.ttf", 14);
    self->nodes = tree_read_dir_nodes(self, rend, path, &self->node_num);

    return self;
}


void tree_free(struct Tree* self)
{
    for (int i = 0; i < self->node_num; ++i)
        node_free(self->nodes[i]);

    free(self->nodes);
    free(self);
}


void tree_render(struct Tree* self, SDL_Renderer* rend)
{
    // SDL_Point* passed in will be modified so copy
    SDL_Point start = self->pos;

    for (int i = 0; i < self->node_num; ++i)
        node_render(self->nodes[i], rend, &start);
}


struct Node** tree_read_dir_nodes(struct Tree* self, SDL_Renderer* rend, const char* path, int* count)
{
    // Directories always go on top of files
    int ndirs;
    char** dirs = tree_read_dir_type(self, path, DT_DIR, &ndirs);
    utils_sort_alphabetically(dirs, ndirs);

    int nfiles;
    char** files = tree_read_dir_type(self, path, DT_REG, &nfiles);
    utils_sort_alphabetically(files, nfiles);

    *count = nfiles + ndirs;
    struct Node** nodes = malloc(sizeof(struct Node*) * *count);

    for (int i = 0; i < ndirs; ++i)
        nodes[i] = node_alloc(rend, self->font, dirs[i]);

    for (int i = 0; i < nfiles; ++i)
        nodes[ndirs + i] = node_alloc(rend, self->font, files[i]);

    return nodes;
}


char** tree_read_dir_type(struct Tree* self, const char* path, unsigned char type, int* count)
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
            items[*count - 1] = malloc(sizeof(char) * (strlen(de->d_name) + 1));

            memcpy(items[*count - 1], de->d_name, strlen(de->d_name));
            items[*count - 1][strlen(de->d_name)] = '\0';
        }
    }

    closedir(dir);
    return items;
}

