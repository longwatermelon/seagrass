#include "node.h"
#include "fs.h"
#include "tree.h"
#include "utils.h"
#include <dirent.h>


struct Node* node_alloc(SDL_Renderer* rend, TTF_Font* font, char* path, unsigned char type)
{
    struct Node* self = malloc(sizeof(struct Node));
    self->path = path;
    self->type = type;

    char* name = fs_filename(self->path);
    self->tex = utils_render_text(rend, name, font, (SDL_Color){ 255, 255, 255 });
    free(name);

    self->nodes = malloc(0);
    self->node_num = 0;

    self->opened = false;

    return self;
}


void node_free(struct Node* self)
{
    for (int i = 0; i < self->node_num; ++i)
        node_free(self->nodes[i]);

    free(self->nodes);

    SDL_DestroyTexture(self->tex);
    free(self->path);

    free(self);
}


void node_render(struct Node* self, SDL_Renderer* rend, SDL_Point* p, SDL_Texture** tex, int top_y)
{
    SDL_Rect rect = { .x = p->x, .y = p->y };
    SDL_QueryTexture(self->tex, 0, 0, &rect.w, &rect.h);

    if (p->y >= top_y)
    {
        SDL_RenderCopy(rend, self->tex, 0, &rect);

        SDL_Rect icon_rect = { .x = p->x - 20, .y = p->y, .w = 16, .h = 16 };

        int index;

        switch (self->type)
        {
        case DT_DIR:
        {
            index = TEX_FOLDER;

            SDL_Rect arrow_rect = { .x = p->x - 40 + 2, .y = p->y + 2, .w = 12, .h = 12 };
            SDL_RenderCopy(rend, (self->opened ? tex[TEX_ARR_D] : tex[TEX_ARR_R]), 0, &arrow_rect);
        } break;
        case DT_REG:
            index = TEX_FILE;
            break;
        }

        SDL_RenderCopy(rend, tex[index], 0, &icon_rect);
    }
    
    p->y += rect.h;
    p->x += 20;

    for (int i = 0; i < self->node_num; ++i)
        node_render(self->nodes[i], rend, p, tex, top_y);

    p->x -= 20;
}


void node_toggle_opened(struct Node* self, SDL_Renderer* rend, TTF_Font* font)
{
    if (self->opened)
    {
        for (int i = 0; i < self->node_num; ++i)
            node_free(self->nodes[i]);

        free(self->nodes);

        self->nodes = malloc(0);
        self->node_num = 0;

        self->opened = false;
    }
    else
    {
        self->opened = true;
        self->nodes = node_read_dir_node(self, rend, font, self->path, &self->node_num);
    }
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


void node_lowest_y(struct Node* self, int* y)
{
    int h;
    SDL_QueryTexture(self->tex, 0, 0, 0, &h);

    *y += h;

    for (int i = 0; i < self->node_num; ++i)
        node_lowest_y(self->nodes[i], y);
}


struct Node** node_read_dir_node(struct Node* self, SDL_Renderer* rend, TTF_Font* font, const char* path, int* count)
{
    // Directories always go on top of files
    int ndirs;
    char** dirs = fs_read_dir(path, DT_DIR, &ndirs);
    utils_sort_alphabetically(dirs, ndirs);

    int nfiles;
    char** files = fs_read_dir(path, DT_REG, &nfiles);
    utils_sort_alphabetically(files, nfiles);

    *count = nfiles + ndirs;
    struct Node** nodes = malloc(sizeof(struct Node*) * *count);

    for (int i = 0; i < ndirs; ++i)
        nodes[i] = node_alloc(rend, font, dirs[i], DT_DIR);

    for (int i = 0; i < nfiles; ++i)
        nodes[ndirs + i] = node_alloc(rend, font, files[i], DT_REG);

    free(files);
    free(dirs);

    return nodes;
}

