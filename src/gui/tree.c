#include "tree.h"
#include "utils.h"
#include <dirent.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


struct Tree* tree_alloc(SDL_Point pos, const char* path, SDL_Renderer* rend)
{
    struct Tree* self = malloc(sizeof(struct Tree));
    self->path = path;
    self->pos = pos;
    self->path = path;

    self->font = TTF_OpenFont("res/font.ttf", 14);
    TTF_SizeText(self->font, " ", &self->char_dim.x, &self->char_dim.y);

    char* abspath = realpath(path, 0);
    self->root = node_alloc(rend, self->font, abspath, DT_DIR);

    node_toggle_opened(self->root, rend, self->font);

    self->ntextures = 2;
    self->textures[TEX_FOLDER] = IMG_LoadTexture(rend, "res/folder.png");
    self->textures[TEX_FILE] = IMG_LoadTexture(rend, "res/file.png");

    self->highlighted_y = -1;

    return self;
}


void tree_free(struct Tree* self)
{
    node_free(self->root);
    TTF_CloseFont(self->font);

    for (int i = 0; i < self->ntextures; ++i)
        SDL_DestroyTexture(self->textures[i]);

    free(self);
}


int tree_render(struct Tree* self, SDL_Renderer* rend)
{
    // SDL_Point* passed in will be modified so copy
    SDL_Point start = (SDL_Point){ .x = self->pos.x, .y = self->pos.y - self->char_dim.y };
    node_render(self->root, rend, &start, self->textures);

    if (self->highlighted_y != -1)
    {
        SDL_Rect highlight = {
            .x = self->pos.x,
            .y = self->highlighted_y,
            .w = 300,
            .h = self->char_dim.y
        };

        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 125);
        SDL_RenderFillRect(rend, &highlight);
        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
    }

    return start.y;
}


void tree_render_highlight(struct Tree* self, SDL_Renderer* rend, int my, int lowest_y)
{
    if (my > lowest_y || my <= self->pos.y)
        return;

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 75);

    SDL_Rect rect = {
        .x = self->pos.x,
        .y = my - (my % self->char_dim.y),
        .w = 300,
        .h = self->char_dim.y
    };

    SDL_RenderFillRect(rend, &rect);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
}


struct Node* tree_clicked(struct Tree* self, int mx, int my)
{
    SDL_Point start = (SDL_Point){ .x = self->pos.x, .y = self->pos.y - self->char_dim.y };
    return node_find_rect(self->root, &start, my);
}

