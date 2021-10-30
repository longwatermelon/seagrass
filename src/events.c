#include "events.h"
#include "prog.h"
#include "gui/textbox.h"
#include "gui/utils.h"
#include <SDL2/SDL_keycode.h>
#include <sys/stat.h>

bool g_mouse_down = false;
bool g_ctrl_down = false;


void events_base(struct Prog* p, SDL_Event* evt)
{
    while (SDL_PollEvent(evt))
    {
        switch (evt->type)
        {
        case SDL_QUIT:
            p->running = false;
            break;
        case SDL_TEXTINPUT:
            if (p->selected_textbox)
                textbox_add_char(p->selected_textbox, p->rend, evt->text.text[0]);
            break;
        case SDL_KEYDOWN:
            events_keydown(p, evt);
            break;
        case SDL_MOUSEBUTTONDOWN:
            events_mouse(p, evt);
            break;
        case SDL_MOUSEBUTTONUP:
            events_mouse_release(p, evt);
            break;
        case SDL_MOUSEWHEEL:
            events_mousewheel(p, evt);
            break;
        }
    }

    if (g_mouse_down)
    {
        if (p->selected_textbox)
        {
            int mx, my;
            SDL_GetMouseState(&mx, &my);

            textbox_cursor_follow_mouse(p->selected_textbox, mx, my);
        }
    }
}


void events_keydown(struct Prog* p, SDL_Event* evt)
{
    if (p->selected_textbox)
    {
        switch (evt->key.keysym.sym)
        {
        case SDLK_RIGHT:
            p->selected_textbox->highlighting = false;
            textbox_move_cursor(p->selected_textbox, 1, 0);
            break;
        case SDLK_LEFT:
            p->selected_textbox->highlighting = false;
            textbox_move_cursor(p->selected_textbox, -1, 0);
            break;
        case SDLK_UP:
            p->selected_textbox->highlighting = false;
            textbox_move_cursor(p->selected_textbox, 0, -1);
            break;
        case SDLK_DOWN:
            p->selected_textbox->highlighting = false;
            textbox_move_cursor(p->selected_textbox, 0, 1);
            break;
        case SDLK_TAB:
            for (int i = 0; i < 4; ++i)
                textbox_add_char(p->selected_textbox, p->rend, ' ');
            break;
        }

        switch (evt->key.keysym.scancode)
        {
        case SDL_SCANCODE_RETURN:
            textbox_add_nl(p->selected_textbox, p->rend);
            break;
        case SDL_SCANCODE_BACKSPACE:
            textbox_del_char(p->selected_textbox, p->rend);
            break;
        default:
            break;
        }
    }

    switch (evt->key.keysym.sym)
    {
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        g_ctrl_down = true;
        break;
    case SDLK_s:
        if (g_ctrl_down)
            prog_save_file(p);
        break;
    }
}


void events_keyup(struct Prog* p, SDL_Event* evt)
{
    switch (evt->key.keysym.sym)
    {
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        g_ctrl_down = false;
        break;
    }
}


void events_mouse(struct Prog* p, SDL_Event* evt)
{
    if (evt->button.button == SDL_BUTTON_LEFT)
        events_mouse_left(p, evt);
}


void events_mouse_left(struct Prog* p, SDL_Event* evt)
{
    g_mouse_down = true;

    if (p->binary_show_warning)
    {
        events_mouse_left_binary_warn(p, evt);
    }
    else
    {
        events_mouse_left_textbox(p, evt);
        events_mouse_left_scrollbar(p, evt);
    }

    events_mouse_left_tree(p, evt);
}


void events_mouse_left_textbox(struct Prog* p, SDL_Event* evt)
{
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    if (utils_p_in_rect(mouse, p->main_textbox->rect))
        p->selected_textbox = p->main_textbox;
    else
        p->selected_textbox = 0;

    if (p->selected_textbox)
    {
        p->selected_textbox->highlighting = true;

        textbox_cursor_follow_mouse(p->selected_textbox, mouse.x, mouse.y);
        p->selected_textbox->highlight_begin = p->selected_textbox->cursor_pos;
    }
}


void events_mouse_left_scrollbar(struct Prog* p, SDL_Event* evt)
{
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    if (utils_p_in_rect(mouse, p->main_scrollbar->rect))
    {
        p->selected_textbox = 0;
        p->main_scrollbar->held = true;
        p->main_scrollbar->mouse_dist = mouse.y - p->main_scrollbar->rect.y;
    }
}


void events_mouse_left_tree(struct Prog* p, SDL_Event* evt)
{
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    struct Node* clicked = tree_clicked(p->file_tree, mouse.x, mouse.y);

    if (clicked && mouse.x < p->main_textbox->rect.x)
    {
        p->file_tree->highlighted_y = mouse.y - (mouse.y % p->file_tree->char_dim.y);

        struct stat sb;
        stat(clicked->path, &sb);

        if (S_ISDIR(sb.st_mode))
            node_toggle_opened(clicked, p->rend, p->file_tree->font);
        else if (S_ISREG(sb.st_mode))
            prog_open_file(p, clicked->path);
    }
}


void events_mouse_left_binary_warn(struct Prog* p, SDL_Event* evt)
{
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    button_check_clicked(p->binary_confirm_btn, mouse.x, mouse.y);

    if (p->binary_confirm_btn->down)
        p->binary_confirm_btn_primed = true;
}


void events_mouse_release(struct Prog* p, SDL_Event* evt)
{
    g_mouse_down = false;
    p->main_scrollbar->held = false;

    if (p->selected_textbox)
    {
        if (p->selected_textbox->highlight_begin.x == p->selected_textbox->cursor_pos.x &&
            p->selected_textbox->highlight_begin.y == p->selected_textbox->cursor_pos.y)
        {
            p->selected_textbox->highlighting = false;
            p->selected_textbox->highlight_begin = (SDL_Point){ .x = 0, .y = 0 };
        }
    }

    p->binary_confirm_btn->down = false;
}


void events_mousewheel(struct Prog* p, SDL_Event* evt)
{
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    if (utils_p_in_rect(mouse, p->main_textbox->rect))
        textbox_move_view(p->main_textbox, 0, -evt->wheel.y);

    int wx, wy;
    SDL_GetWindowSize(p->window, &wx, &wy);
    SDL_Rect tree = { .x = p->file_tree->orig_pos.x, .y = p->file_tree->orig_pos.y, .w = 300, .h = wy - p->file_tree->orig_pos.y };

    if (utils_p_in_rect(mouse, tree))
        tree_scroll(p->file_tree, evt->wheel.y, wy);
}

