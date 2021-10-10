#include "events.h"
#include "prog.h"
#include "gui/textbox.h"
#include "gui/utils.h"


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
        case SDL_MOUSEWHEEL:
            events_mousewheel(p, evt);
            break;
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
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    if (utils_p_in_rect(mouse, p->main_textbox->rect))
        p->selected_textbox = p->main_textbox;
    else
        p->selected_textbox = 0;

    if (p->selected_textbox)
    {
        SDL_Point rel = {
            .x = mouse.x - p->selected_textbox->rect.x,
            .y = mouse.y - p->selected_textbox->rect.y
        };

        SDL_Point dst = {
            .x = (rel.x - (rel.x % p->selected_textbox->text->char_dim.x)) / p->selected_textbox->text->char_dim.x + p->selected_textbox->view_pos.x,
            .y = (rel.y - (rel.y % p->selected_textbox->text->char_dim.y)) / p->selected_textbox->text->char_dim.y + p->selected_textbox->view_pos.y
        };

        textbox_move_cursor(p->selected_textbox,
            dst.x - p->selected_textbox->cursor_pos.x,
            dst.y - p->selected_textbox->cursor_pos.y
        );
    }
}


void events_mouse_right(struct Prog* p, SDL_Event* evt)
{
}


void events_mousewheel(struct Prog* p, SDL_Event* evt)
{
    if (p->selected_textbox)
    {
        textbox_move_view(p->selected_textbox, 0, -evt->wheel.y);
    }
}

