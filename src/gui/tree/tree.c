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
    TTF_SizeText(self->font, " ", &self->char_dim.x, &self->char_dim.y);

    char* abspath = realpath(path, 0);
    self->root = node_alloc(rend, self->font, abspath);

    node_read_subnodes(self->root, rend, self->font);

    return self;
}


void tree_free(struct Tree* self)
{
    node_free(self->root);
    TTF_CloseFont(self->font);
    free(self);
}


void tree_render(struct Tree* self, SDL_Renderer* rend)
{
    // SDL_Point* passed in will be modified so copy
    SDL_Point start = (SDL_Point){ .x = self->pos.x, .y = self->pos.y - self->char_dim.y };
    node_render(self->root, rend, &start);
}



struct Node* tree_clicked(struct Tree* self, int mx, int my)
{
    SDL_Point start = (SDL_Point){ .x = self->pos.x, .y = self->pos.y - self->char_dim.y };
    return node_find_rect(self->root, &start, my);
}

