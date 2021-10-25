#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


SDL_Texture* utils_render_text(SDL_Renderer* rend, const char* text, TTF_Font* font, SDL_Color color)
{
    if (strlen(text) == 0)
        return 0;

    SDL_Surface* surf = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);

    SDL_FreeSurface(surf);
    return tex;
}


char* utils_substr(const char* str, int start, int end)
{
    char* sub = malloc(sizeof(char) * (end - start + 1));
    memcpy(sub, &str[start], end - start);
    sub[end - start] = '\0';

    return sub;
}


char** utils_split_str(const char* str, char delim, int* count)
{
    char** list = malloc(0);
    *count = 0;

    int prev = 0;

    for (int i = 0; i <= strlen(str); ++i)
    {
        if (str[i] == delim || str[i] == '\0')
        {
            char* sub = utils_substr(str, prev, i);

            list = realloc(list, sizeof(char*) * ++*count);
            list[*count - 1] = sub;

            prev = i + 1;

            if (i + 1 == strlen(str))
                break;
        }
    }

    return list;
}


bool utils_p_in_rect(SDL_Point p, SDL_Rect rect)
{
    return p.x >= rect.x && p.x <= rect.x + rect.w &&
           p.y >= rect.y && p.y <= rect.y + rect.h;
}


void utils_sort_alphabetically(char** arr, int len)
{
    for (int i = 0; i < len; ++i)
    {
        for (int j = i + 1; j < len; ++j)
        {
            if (strcmp(arr[i], arr[j]) > 0)
            {
                char* tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}


char* utils_find_resource(const char* name)
{
    DIR* dir = opendir("/usr/share/seagrass");

    // Use resources from /usr/share if possible
    if (dir)
    {
        closedir(dir);

        int len = strlen("/usr/share/seagrass/") + strlen(name) + 1;
        char* path = malloc(sizeof(char) * len);

        sprintf(path, "/usr/share/seagrass/%s", name);
        return path;
    }
    else
    {
        char* path = malloc(sizeof(char) * (strlen(name) + 1));
        sprintf(path, "%s", name);
        return path;
    }
}

