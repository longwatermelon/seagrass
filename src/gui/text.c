#include "text.h"
#include "utils.h"


struct Text* text_alloc(SDL_Renderer* rend, SDL_Point pos, const char* contents, TTF_Font* font, SDL_Color color)
{
    struct Text* self = malloc(sizeof(struct Text));

    self->pos = pos;
    self->contents = contents;
    self->font = font;
    self->color = color;

    TTF_SizeText(self->font, self->contents, &self->char_dim.x, &self->char_dim.y);

    self->textures = text_split_str_into_textures(self, rend, &self->ntextures);

    return self;
}


void text_free(struct Text* self)
{
    for (int i = 0; i < self->ntextures; ++i)
    {
        if (self->textures[i])
            SDL_DestroyTexture(self->textures[i]);
    }

    free(self->textures);
    free(self);
}


void text_render(struct Text* self, SDL_Renderer* rend)
{
    for (int i = 0; i < self->ntextures; ++i)
    {
        SDL_Rect dst = {
            .x = self->pos.x,
            .y = self->pos.y + i * self->char_dim.y
        };

        SDL_QueryTexture(self->textures[i], 0, 0, &dst.w, &dst.h);
        SDL_RenderCopy(rend, self->textures[i], 0, &dst);
    }
}


SDL_Texture** text_split_str_into_textures(struct Text* self, SDL_Renderer* rend, int* count)
{
    SDL_Texture** textures = malloc(0);
    *count = 0;

    int prev = 0;

    for (int i = 0; i <= strlen(self->contents); ++i)
    {
        if (self->contents[i] == '\n' || self->contents[i] == '\0')
        {
            char* line = utils_substr(self->contents, prev, i);

            textures = realloc(textures, sizeof(SDL_Texture*) * ++*count);
            textures[*count - 1] = text_render_text(self, rend, line);

            free(line);
            prev = i + 1;

            if (i + 1 == strlen(self->contents))
                break;
        }
    }

    return textures;
}


static SDL_Texture* text_render_text(struct Text* self, SDL_Renderer* rend, const char* text)
{
    if (strlen(text) == 0)
        return 0;

    SDL_Surface* surf = TTF_RenderText_Blended(self->font, text, self->color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);

    SDL_FreeSurface(surf);
    return tex;
}

