#include "textbox.h"
#include "utils.h"


struct Textbox* textbox_alloc(SDL_Rect rect, SDL_Renderer* rend, TTF_Font* font, SDL_Color color)
{
    struct Textbox* self = malloc(sizeof(struct Textbox));
    self->rect = rect;

    self->text = text_alloc(rend, (SDL_Point){ rect.x, rect.y }, "", font, (SDL_Color){ 255, 255, 255 });
    self->bg_color = color;

    self->cursor_pos = (SDL_Point){ .x = 0, .y = 0 };

    self->view_pos = (SDL_Point){ .x = 0, .y = 0 };

    return self;
}


void textbox_free(struct Textbox* self)
{
    text_free(self->text);
    free(self);
}


void textbox_render(struct Textbox* self, SDL_Renderer* rend)
{
    SDL_SetRenderDrawColor(rend, self->bg_color.r, self->bg_color.g, self->bg_color.b, 255);
    SDL_RenderFillRect(rend, &self->rect);

    text_render(self->text, rend, self->view_pos, &self->rect);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_Point pos = textbox_char_to_pix_pos(self, self->cursor_pos);

    SDL_Rect cursor = {
        .x = pos.x - (self->view_pos.x * self->text->char_dim.x),
        .y = pos.y - (self->view_pos.y * self->text->char_dim.y),
        .w = 2,
        .h = self->text->char_dim.y
    };

    SDL_RenderFillRect(rend, &cursor);
}



void textbox_move_cursor(struct Textbox* self, int x, int y)
{
    if (self->cursor_pos.x + x >= 0 &&
        self->cursor_pos.x + x <= strlen(self->text->lines[self->cursor_pos.y]))
        self->cursor_pos.x += x;

    if (self->cursor_pos.y + y >= 0 &&
        self->cursor_pos.y + y < self->text->nlines)
    {
        self->cursor_pos.y += y;
        textbox_cond_jump_to_eol(self);
    }

    textbox_move_view_cursor(self);
}


void textbox_cond_jump_to_eol(struct Textbox* self)
{
    if (self->cursor_pos.x > strlen(self->text->lines[self->cursor_pos.y]))
    {
        self->cursor_pos.x = strlen(self->text->lines[self->cursor_pos.y]);
        textbox_move_view_cursor(self);
    }
}


void textbox_add_char(struct Textbox* self, SDL_Renderer* rend, char c)
{
    char* curr_line = self->text->lines[self->cursor_pos.y];
    int len = strlen(curr_line) + 1;

    curr_line = realloc(curr_line, sizeof(char) * (len + 1));
    curr_line[len] = '\0';

    memcpy(&curr_line[self->cursor_pos.x + 1],
           &curr_line[self->cursor_pos.x],
           sizeof(char) * (strlen(curr_line) - self->cursor_pos.x));

    curr_line[self->cursor_pos.x] = c;

    text_redo_texture(self->text, rend, self->cursor_pos.y, curr_line);
    free(curr_line);

    textbox_move_cursor(self, 1, 0);
}


void textbox_add_nl(struct Textbox* self, SDL_Renderer* rend)
{
    char* line = self->text->lines[self->cursor_pos.y];

    char* copied = utils_substr(line, self->cursor_pos.x, strlen(line));
    text_insert_texture(self->text, rend, self->cursor_pos.y + 1, copied);
    free(copied);

    char* new_line = utils_substr(line, 0, self->cursor_pos.x);
    text_redo_texture(self->text, rend, self->cursor_pos.y, new_line);
    free(new_line);
    free(line);

    textbox_move_cursor(self, -self->cursor_pos.x, 1);
}


void textbox_del_char(struct Textbox* self, SDL_Renderer* rend)
{
    if (self->cursor_pos.x == 0)
    {
        if (self->cursor_pos.y == 0)
            return;

        textbox_move_cursor(self, 0, -1);
        textbox_move_cursor(self, strlen(self->text->lines[self->cursor_pos.y]), 0);
        text_remove_texture(self->text, rend, self->cursor_pos.y + 1);
    }
    else
    {
        char* line = self->text->lines[self->cursor_pos.y];
        memcpy(&line[self->cursor_pos.x - 1],
               &line[self->cursor_pos.x],
               sizeof(char) * (strlen(line) - self->cursor_pos.x));

        int len = strlen(line) - 1;
        line = realloc(line, sizeof(char) * (len + 1));
        line[len] = '\0';

        text_redo_texture(self->text, rend, self->cursor_pos.y, line);
        textbox_move_cursor(self, -1, 0);

        free(line);
    }
}


void textbox_del_nl(struct Textbox* self, SDL_Renderer* rend)
{
}


void textbox_move_view(struct Textbox* self, int x, int y)
{
    self->view_pos.x += x;

    if (self->view_pos.x < 0)
        self->view_pos.x = 0;

    self->view_pos.y += y;

    if (self->view_pos.y < 0)
        self->view_pos.y = 0;
}


void textbox_move_view_cursor(struct Textbox* self)
{
    SDL_Point char_size = {
        .x = self->rect.w / self->text->char_dim.x - 1,
        .y = self->rect.h / self->text->char_dim.y - 1
    };

    SDL_Point moved = (SDL_Point){ .x = 0, .y = 0 };

    if (self->cursor_pos.x - self->view_pos.x < 0)
        moved.x = self->cursor_pos.x - self->view_pos.x - 6;

    if (self->cursor_pos.y - self->view_pos.y < 0)
        moved.y = self->cursor_pos.y - self->view_pos.y - 6;

    if (self->cursor_pos.x - self->view_pos.x > char_size.x)
        moved.x = self->cursor_pos.x - (self->view_pos.x + char_size.x) + 6;

    if (self->cursor_pos.y - self->view_pos.y > char_size.y)
        moved.y = self->cursor_pos.y - (self->view_pos.y + char_size.y) + 6;

    textbox_move_view(self, moved.x, moved.y);
}


static SDL_Point textbox_char_to_pix_pos(struct Textbox* self, SDL_Point pos)
{
    return (SDL_Point){
        .x = self->rect.x + (pos.x * self->text->char_dim.x),
        .y = self->rect.y + (pos.y * self->text->char_dim.y)
    };
}

