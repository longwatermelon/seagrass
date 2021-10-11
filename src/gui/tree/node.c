#include "node.h"
#include "../utils.h"
#include <dirent.h>


struct Node* node_alloc(SDL_Renderer* rend, TTF_Font* font, char* name)
{
    struct Node* self = malloc(sizeof(struct Node));
    self->name = name;
    self->tex = utils_render_text(rend, name, font, (SDL_Color){ 255, 255, 255 });

    self->nodes = malloc(0);
    self->node_num = 0;

    return self;
}


void node_free(struct Node* self)
{
    for (int i = 0; i < self->node_num; ++i)
        node_free(self->nodes[i]);

    free(self->nodes);

    SDL_DestroyTexture(self->tex);
    free(self->name);

    free(self);
}


void node_render(struct Node* self, SDL_Renderer* rend, SDL_Point* p)
{
    SDL_Rect rect = { .x = p->x, .y = p->y };
    SDL_QueryTexture(self->tex, 0, 0, &rect.w, &rect.h);

    SDL_RenderCopy(rend, self->tex, 0, &rect);
    p->y += rect.h;
    p->x += 10;

    for (int i = 0; i < self->node_num; ++i)
        node_render(self->nodes[i], rend, p);

    p->x -= 10;
}


void node_read_subnodes(struct Node* self, SDL_Renderer* rend, TTF_Font* font)
{
    self->nodes = node_read_dir_node(self, rend, font, self->name, &self->node_num);
}


struct Node* node_find_rect(struct Node* self, SDL_Point* start, int find_y)
{
    int h;
    SDL_QueryTexture(self->tex, 0, 0, 0, &h);

    if (find_y > start->y && find_y < start->y + h)
        return self;

    start->y += h;

    for (int i = 0; i < self->node_num; ++i)
    {
        struct Node* n = node_find_rect(self->nodes[i], start, find_y);

        if (n)
            return n;
    }

    return 0;
}


static struct Node** node_read_dir_node(struct Node* self, SDL_Renderer* rend, TTF_Font* font, const char* path, int* count)
{
    // Directories always go on top of files
    int ndirs;
    char** dirs = node_read_dir_type(self, path, DT_DIR, &ndirs);
    utils_sort_alphabetically(dirs, ndirs);

    int nfiles;
    char** files = node_read_dir_type(self, path, DT_REG, &nfiles);
    utils_sort_alphabetically(files, nfiles);

    *count = nfiles + ndirs;
    struct Node** nodes = malloc(sizeof(struct Node*) * *count);

    for (int i = 0; i < ndirs; ++i)
        nodes[i] = node_alloc(rend, font, dirs[i]);

    for (int i = 0; i < nfiles; ++i)
        nodes[ndirs + i] = node_alloc(rend, font, files[i]);

    free(files);
    free(dirs);

    return nodes;
}


static char** node_read_dir_type(struct Node* self, const char* path, unsigned char type, int* count)
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

