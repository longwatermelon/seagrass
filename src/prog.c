#include "prog.h"
#include "events.h"
#include "utils.h"
#include "gui/text.h"
#include "gui/textbox.h"


struct Prog* prog_alloc()
{
    struct Prog* self = malloc(sizeof(struct Prog));
    self->running = true;

    self->window = SDL_CreateWindow("Seagrass", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    self->rend = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    self->font = TTF_OpenFont("res/font.ttf", 16);

    int wx, wy;
    SDL_GetWindowSize(self->window, &wx, &wy);

    self->main_textbox = textbox_alloc((SDL_Rect){ .x = 300, .y = 100, .w = wx - 300, .h = wy - 100 }, self->rend, self->font, (SDL_Color){ 50, 50, 50 });
    self->main_scrollbar = scrollbar_alloc((SDL_Rect){ .x = wx - 20, .y = 100, .w = 20, .h = wy - 100 });
    self->file_tree = tree_alloc((SDL_Point){ 0, 100 }, ".", self->rend);

    self->selected_textbox = 0;

    return self;
}


void prog_free(struct Prog* self)
{
    textbox_free(self->main_textbox);
    scrollbar_free(self->main_scrollbar);
    tree_free(self->file_tree);

    TTF_CloseFont(self->font);

    SDL_DestroyRenderer(self->rend);
    SDL_DestroyWindow(self->window);

    free(self);
}


void prog_mainloop(struct Prog* self)
{
    SDL_Event evt;

    while (self->running)
    {
        events_base(self, &evt);

        prog_mainloop_textbox(self);
        prog_mainloop_scrollbar(self);
    
        prog_render(self);
    }
}


void prog_render(struct Prog* self)
{
    SDL_RenderClear(self->rend);

    tree_render(self->file_tree, self->rend);
    textbox_render(self->main_textbox, self->rend, self->main_textbox == self->selected_textbox);
    textbox_render_highlight(self->main_textbox, self->rend);

    scrollbar_render(self->main_scrollbar, self->rend);

    SDL_SetRenderDrawColor(self->rend, 20, 20, 20, 255);
    SDL_RenderPresent(self->rend);
}


void prog_mainloop_textbox(struct Prog* self)
{
    int wx, wy;
    SDL_GetWindowSize(self->window, &wx, &wy);

    self->main_textbox->rect.w = wx - self->main_textbox->rect.x - 20;
    self->main_textbox->rect.h = wy - self->main_textbox->rect.y;
}


void prog_mainloop_scrollbar(struct Prog* self)
{
    int wx, wy;
    SDL_GetWindowSize(self->window, &wx, &wy);

    self->main_scrollbar->rect.x = wx - self->main_scrollbar->rect.w;
    self->main_scrollbar->rect.h = wy - self->main_scrollbar->rect.y;

    int rows = (self->main_textbox->rect.h - (self->main_textbox->rect.h % self->main_textbox->text->char_dim.y)) / self->main_textbox->text->char_dim.y;

    scrollbar_update_units(self->main_scrollbar, self->main_textbox->text->nlines + rows - 1, rows);
    scrollbar_follow_mouse(self->main_scrollbar);

    if (self->main_scrollbar->held)
        self->main_textbox->view_pos.y = self->main_scrollbar->bar_top_units;
    else
        self->main_scrollbar->bar_top_units = self->main_textbox->view_pos.y;
}


void prog_open_file(struct Prog* self, const char* fp)
{
    char* contents = utils_read_file(fp);
    textbox_set_text(self->main_textbox, self->rend, self->font, contents);
    free(contents);
}

