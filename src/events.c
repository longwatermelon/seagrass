#include "events.h"
#include "prog.h"
#include "gui/textbox.h"
#include "gui/utils.h"

bool g_mouse_down = false;


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
            p->selected_textbox->highlight_end = p->selected_textbox->cursor_pos;
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
            textbox_move_cursor(p->selected_textbox, 1, 0);
            break;
        case SDLK_LEFT:
            textbox_move_cursor(p->selected_textbox, -1, 0);
            break;
        case SDLK_UP:
            textbox_move_cursor(p->selected_textbox, 0, -1);
            break;
        case SDLK_DOWN:
            textbox_move_cursor(p->selected_textbox, 0, 1);
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
}


void events_mouse(struct Prog* p, SDL_Event* evt)
{
    if (evt->button.button == SDL_BUTTON_LEFT)
        events_mouse_left(p, evt);
    else if (evt->button.button == SDL_BUTTON_RIGHT)
        events_mouse_right(p, evt);
}


void events_mouse_left(struct Prog* p, SDL_Event* evt)
{
    g_mouse_down = true;

    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    if (utils_p_in_rect(mouse, p->main_textbox->rect))
        p->selected_textbox = p->main_textbox;
    else
        p->selected_textbox = 0;

    if (p->selected_textbox)
    {
        p->selected_textbox->highlighting = true;

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        textbox_cursor_follow_mouse(p->selected_textbox, mx, my);
        p->selected_textbox->highlight_begin = p->selected_textbox->cursor_pos;
    }
}


void events_mouse_right(struct Prog* p, SDL_Event* evt)
{
}


void events_mouse_release(struct Prog* p, SDL_Event* evt)
{
    g_mouse_down = false;

    if (p->selected_textbox)
    {
        if (p->selected_textbox->highlight_begin.x == p->selected_textbox->highlight_end.x &&
            p->selected_textbox->highlight_begin.y == p->selected_textbox->highlight_end.y)
        {
            p->selected_textbox->highlighting = false;
            p->selected_textbox->highlight_begin = (SDL_Point){ .x = 0, .y = 0 };
            p->selected_textbox->highlight_end = (SDL_Point){ .x = 0, .y = 0 };
        }
    }
}


void events_mousewheel(struct Prog* p, SDL_Event* evt)
{
    if (p->selected_textbox)
    {
        textbox_move_view(p->selected_textbox, 0, -evt->wheel.y);
    }
}

