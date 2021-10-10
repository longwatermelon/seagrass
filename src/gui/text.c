#include "text.h"
#include "utils.h"


struct Text* text_alloc(SDL_Renderer* rend, SDL_Point pos, const char* contents, TTF_Font* font, SDL_Color color)
{
    struct Text* self = malloc(sizeof(struct Text));

    self->pos = pos;
    self->font = font;
    self->color = color;

    self->lines = utils_split_str(contents, '\n', &self->nlines);
    self->textures = text_split_str_into_textures(self, rend);

    TTF_SizeText(self->font, " ", &self->char_dim.x, &self->char_dim.y);

    return self;
}


void text_free(struct Text* self)
{
    for (int i = 0; i < self->nlines; ++i)
    {
        if (self->textures[i])
            SDL_DestroyTexture(self->textures[i]);

        free(self->lines[i]);
    }

    free(self->lines);
    free(self->textures);
    free(self);
}


void text_render(struct Text* self, SDL_Renderer* rend, SDL_Point view_pos, SDL_Rect* enclosing_rect)
{
    SDL_Point pix_view_point = {
        .x = view_pos.x * self->char_dim.x,
        .y = view_pos.y * self->char_dim.y
    };

    for (int i = 0; i < self->nlines; ++i)
    {
        if (!self->textures[i])
            continue;

        SDL_Rect dst = {
            .x = self->pos.x,
            .y = self->pos.y + i * self->char_dim.y - pix_view_point.y
        };

        if (dst.y < self->pos.y ||
            (enclosing_rect && dst.y > enclosing_rect->y + enclosing_rect->h))
            continue;

        SDL_QueryTexture(self->textures[i], 0, 0, &dst.w, &dst.h);

        SDL_Rect src = { .x = pix_view_point.x, .y = 0, .w = dst.w, .h = dst.h };

        src.w -= src.x;
        dst.w -= src.x;

        if (enclosing_rect)
        {
            if (src.w > enclosing_rect->w)
            {
                int diff = src.w - enclosing_rect->w;
                src.w -= diff;
                dst.w -= diff;
            }
        }

        SDL_RenderCopy(rend, self->textures[i], &src, &dst);
    }
}


void text_redo_textures(struct Text* self, SDL_Renderer* rend)
{
    for (int i = 0; i < self->nlines; ++i)
    {
        if (self->textures[i])
            SDL_DestroyTexture(self->textures[i]);
    }

    free(self->textures);

    self->textures = text_split_str_into_textures(self, rend);
}


void text_redo_texture(struct Text* self, SDL_Renderer* rend, int index, const char* text)
{
    if (self->textures[index])
        SDL_DestroyTexture(self->textures[index]);

    self->textures[index] = utils_render_text(rend, text, self->font, self->color);

    self->lines[index] = malloc(sizeof(char) * (strlen(text) + 1));
    memcpy(self->lines[index], text, strlen(text));
    self->lines[index][strlen(text)] = '\0';
}


void text_insert_texture(struct Text* self, SDL_Renderer* rend, int index, const char* text)
{
    self->lines = realloc(self->lines, sizeof(char*) * (self->nlines + 1));

    memcpy(&self->lines[index + 1],
           &self->lines[index],
           sizeof(char*) * (self->nlines - index));

    self->lines[index] = malloc(sizeof(char) * (strlen(text) + 1));
    memcpy(self->lines[index], text, sizeof(char) * strlen(text));
    self->lines[index][strlen(text)] = '\0';


    self->textures = realloc(self->textures, sizeof(SDL_Texture*) * (self->nlines + 1));

    memcpy(&self->textures[index + 1],
           &self->textures[index],
           sizeof(SDL_Texture*) * (self->nlines - index));

    self->textures[index] = utils_render_text(rend, text, self->font, self->color);

    ++self->nlines;
}


static SDL_Texture** text_split_str_into_textures(struct Text* self, SDL_Renderer* rend)
{
    SDL_Texture** textures = malloc(sizeof(SDL_Texture*) * self->nlines);

    for (int i = 0; i < self->nlines; ++i)
        textures[i] = utils_render_text(rend, self->lines[i], self->font, self->color);

    return textures;
}

