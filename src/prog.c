#include "prog.h"
#include "events.h"
#include "gui/button.h"
#include "gui/utils.h"
#include "gui/text.h"
#include "gui/textbox.h"
#include "gui/fs.h"


struct Prog* prog_alloc()
{
    struct Prog* self = malloc(sizeof(struct Prog));
    self->running = true;

    self->window = SDL_CreateWindow("Seagrass", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    self->rend = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    char* tmp = utils_find_resource("res/font.ttf");
    self->font = TTF_OpenFont(tmp, 16);
    free(tmp);

    int wx, wy;
    SDL_GetWindowSize(self->window, &wx, &wy);

    self->main_textbox = textbox_alloc((SDL_Rect){ .x = 300, .y = 50, .w = wx - 300, .h = wy - 50 }, self->rend, self->font, (SDL_Color){ 30, 30, 30 });
    self->main_scrollbar = scrollbar_alloc((SDL_Rect){ .x = wx - 15, .y = 50, .w = 15, .h = wy - 50 });
    self->file_tree = tree_alloc((SDL_Point){ 0, 50 }, ".", self->rend);

    self->selected_textbox = 0;

    self->opened_file[0] = '\0';

    self->binary_show_warning = false;
    self->binary_confirm_btn = button_alloc((SDL_Rect){
        .x = self->main_textbox->rect.x + 40,
        .y = self->main_textbox->rect.y + 40, 
        .w = 150, .h = 20 }, "View anyways", (SDL_Color){ 100, 100, 100 }, self->rend, self->font);

    self->binary_warning_text = text_alloc(self->rend, (SDL_Point){
        .x = self->main_textbox->rect.x + 40,
        .y = self->main_textbox->rect.y + 10 }, "This file contains non ascii characters and won't be rendered correctly.", self->font, (SDL_Color){ 255, 255, 255 });

    return self;
}


void prog_free(struct Prog* self)
{
    button_free(self->binary_confirm_btn);
    text_free(self->binary_warning_text);

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

        if (!self->binary_show_warning)
        {
            prog_mainloop_textbox(self);
            prog_mainloop_scrollbar(self);
        }
        else
        {
            prog_mainloop_binary_warning(self);
        }

        prog_render(self);
    }
}


void prog_render(struct Prog* self)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    SDL_RenderClear(self->rend);

    int lowest_y = tree_render(self->file_tree, self->rend);

    if (mx < self->main_textbox->rect.x)
        tree_render_highlight(self->file_tree, self->rend, my, lowest_y);

    if (self->binary_show_warning)
    {
        prog_render_binary_warning(self);
    }
    else
    {
        textbox_render(self->main_textbox, self->rend, self->main_textbox == self->selected_textbox);
        textbox_render_highlight(self->main_textbox, self->rend);

        scrollbar_render(self->main_scrollbar, self->rend, mx, my);
    }

    SDL_SetRenderDrawColor(self->rend, 50, 50, 50, 255);
    SDL_RenderPresent(self->rend);
}


void prog_mainloop_textbox(struct Prog* self)
{
    int wx, wy;
    SDL_GetWindowSize(self->window, &wx, &wy);

    self->main_textbox->rect.w = wx - self->main_textbox->rect.x - 15;
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


void prog_mainloop_binary_warning(struct Prog* self)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    button_check_hover(self->binary_confirm_btn, mx, my);

    if (self->binary_confirm_btn_primed && !self->binary_confirm_btn->down)
    {
        self->binary_confirm_btn_primed = false;
        self->binary_show_warning = false;
    }
}


void prog_render_binary_warning(struct Prog* self)
{
    text_render(self->binary_warning_text, self->rend, (SDL_Point){ 0, 0 }, 0);
    button_render(self->binary_confirm_btn, self->rend);
}


void prog_open_file(struct Prog* self, const char* fp)
{
    snprintf(self->opened_file, strlen(fp) + 1, "%s", fp);

    char* contents = fs_read_file(fp);
    self->binary_show_warning = fs_is_binary(contents);

    textbox_set_text(self->main_textbox, self->rend, self->font, contents);
    free(contents);
}


void prog_save_file(struct Prog* self)
{
    if (strlen(self->opened_file) == 0)
        return;

    FILE* fp = fopen(self->opened_file, "w");

    char* out = textbox_get_text(self->main_textbox);
    fprintf(fp, "%s", out);

    free(out);
    fclose(fp);
}

