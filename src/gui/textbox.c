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

    self->highlighting = false;
    self->highlight_begin = (SDL_Point){ 0, 0 };
    self->highlight_end = (SDL_Point){ 0, 0 };

    return self;
}


void textbox_free(struct Textbox* self)
{
    text_free(self->text);
    free(self);
}


void textbox_render(struct Textbox* self, SDL_Renderer* rend, bool render_cursor)
{
    SDL_SetRenderDrawColor(rend, self->bg_color.r, self->bg_color.g, self->bg_color.b, 255);
    SDL_RenderFillRect(rend, &self->rect);

    text_render(self->text, rend, self->view_pos, &self->rect);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_Point pos = textbox_char_to_pix_pos(self, self->cursor_pos);

    if (render_cursor)
    {
        SDL_Rect cursor = {
            .x = pos.x - (self->view_pos.x * self->text->char_dim.x),
            .y = pos.y - (self->view_pos.y * self->text->char_dim.y),
            .w = 2,
            .h = self->text->char_dim.y
        };

        if (cursor.y >= self->rect.y)
            SDL_RenderFillRect(rend, &cursor);
    }
}


void textbox_render_highlight(struct Textbox* self, SDL_Renderer* rend)
{
    if (self->highlighting)
    {
        if (self->highlight_begin.y == self->highlight_end.y)
        {
            textbox_render_highlight_line(self, rend, self->highlight_begin.y, self->highlight_begin.x, self->highlight_end.x);
        }
        else
        {
            SDL_Point* top = (self->highlight_end.y < self->highlight_begin.y ? &self->highlight_end : &self->highlight_begin);
            SDL_Point* bot = (self->highlight_end.y < self->highlight_begin.y ? &self->highlight_begin : &self->highlight_end);

            textbox_render_highlight_line(self, rend, top->y, top->x, strlen(self->text->lines[top->y]));
            textbox_render_highlight_line(self, rend, bot->y, 0, bot->x);

            for (int i = top->y + 1; i < bot->y; ++i)
                textbox_render_highlight_line(self, rend, i, 0, strlen(self->text->lines[i]));
        }
    }
}


void textbox_render_highlight_line(struct Textbox* self, SDL_Renderer* rend, int index, int begin, int end)
{
    int y = self->rect.y + (index - self->view_pos.y) * self->text->char_dim.y;

    int x1 = self->rect.x + (begin - self->view_pos.x) * self->text->char_dim.x;
    int x2 = self->rect.x + (end - self->view_pos.x) * self->text->char_dim.x;

    SDL_Rect rect = {
        .x = x1,
        .y = y,
        .w = x2 - x1,
        .h = self->text->char_dim.y
    };

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend, 203, 241, 244, 100);
    SDL_RenderFillRect(rend, &rect);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
}


void textbox_move_cursor(struct Textbox* self, int x, int y)
{
    self->cursor_pos.x += x;

    if (self->cursor_pos.x < 0)
        self->cursor_pos.x = 0;

    self->cursor_pos.y += y;

    if (self->cursor_pos.y < 0)
        self->cursor_pos.y = 0;

    if (self->cursor_pos.y >= self->text->nlines)
        self->cursor_pos.y = self->text->nlines - 1;

    textbox_cond_jump_to_eol(self);
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


void textbox_cursor_follow_mouse(struct Textbox* self, int mx, int my)
{
    SDL_Point rel = {
        .x = mx - self->rect.x,
        .y = my - self->rect.y
    };

    SDL_Point dst = {
        .x = (rel.x - (rel.x % self->text->char_dim.x)) / self->text->char_dim.x + self->view_pos.x,
        .y = (rel.y - (rel.y % self->text->char_dim.y)) / self->text->char_dim.y + self->view_pos.y
    };

    textbox_move_cursor(self,
        dst.x - self->cursor_pos.x,
        dst.y - self->cursor_pos.y
    );
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
    if (self->highlighting)
    {
        textbox_del_highlighted(self, rend);
        return;
    }

    if (self->cursor_pos.x == 0)
    {
        textbox_del_nl(self, rend);
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
    if (self->cursor_pos.y == 0)
        return;

    char* line = self->text->lines[self->cursor_pos.y - 1];
    char* prev_line = self->text->lines[self->cursor_pos.y];

    textbox_move_cursor(self, 0, -1);
    textbox_move_cursor(self, strlen(line), 0);

    line = realloc(line, sizeof(char) * (strlen(line) + strlen(prev_line) + 1));
    strcat(line, prev_line);

    text_redo_texture(self->text, rend, self->cursor_pos.y, line);
    free(line);

    text_remove_texture(self->text, rend, self->cursor_pos.y + 1);
}


void textbox_move_view(struct Textbox* self, int x, int y)
{
    self->view_pos.x += x;

    if (self->view_pos.x < 0)
        self->view_pos.x = 0;

    self->view_pos.y += y;

    if (self->view_pos.y < 0)
        self->view_pos.y = 0;

    if (self->view_pos.y >= self->text->nlines)
        self->view_pos.y = self->text->nlines - 1;
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


void textbox_del_highlighted(struct Textbox* self, SDL_Renderer* rend)
{
    if (self->highlight_end.y == self->highlight_begin.y)
    {
        int left = self->highlight_begin.x < self->highlight_end.x ? self->highlight_begin.x : self->highlight_end.x;
        int right = self->highlight_begin.x > self->highlight_end.x ? self->highlight_begin.x : self->highlight_end.x;

        textbox_del_highlighted_line(self, rend, self->highlight_begin.y, left, right);
        self->highlighting = false;
        textbox_move_cursor(self, left - self->cursor_pos.x, 0);
    }
    else
    {
        SDL_Point* top = (self->highlight_end.y < self->highlight_begin.y ? &self->highlight_end : &self->highlight_begin);
        SDL_Point* bot = (self->highlight_end.y < self->highlight_begin.y ? &self->highlight_begin : &self->highlight_end);

        textbox_del_highlighted_line(self, rend, top->y, top->x, strlen(self->text->lines[top->y]));
        textbox_del_highlighted_line(self, rend, bot->y, 0, bot->x);

        for (int i = top->y + 1; i < bot->y; ++i)
            text_remove_texture(self->text, rend, top->y + 1);

        char* top_line = self->text->lines[top->y];
        char* next_line = self->text->lines[top->y + 1];

        top_line = realloc(top_line, sizeof(char) * (strlen(top_line) + strlen(next_line) + 1));
        strcat(top_line, next_line);
        text_redo_texture(self->text, rend, top->y, top_line);
        text_remove_texture(self->text, rend, top->y + 1);

        free(top_line);

        self->highlighting = false;
        textbox_move_cursor(self, top->x - self->cursor_pos.x, top->y - self->cursor_pos.y);

    }
}


void textbox_del_highlighted_line(struct Textbox* self, SDL_Renderer* rend, int index, int begin, int end)
{
    char* line = self->text->lines[index];

    int len = strlen(line) - (end - begin);

    memcpy(&line[begin], &line[end], strlen(line) - end);
    line[len] = '\0';
    line = realloc(line, sizeof(char) * (len + 1));

    text_redo_texture(self->text, rend, index, line);
    free(line);
}


static SDL_Point textbox_char_to_pix_pos(struct Textbox* self, SDL_Point pos)
{
    return (SDL_Point){
        .x = self->rect.x + (pos.x * self->text->char_dim.x),
        .y = self->rect.y + (pos.y * self->text->char_dim.y)
    };
}

