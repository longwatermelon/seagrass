#include "utils.h"
#include <stdlib.h>
#include <string.h>


char* utils_substr(const char* str, int start, int end)
{
    char* sub = malloc(sizeof(char) * (end - start + 1));
    memcpy(sub, &str[start], end - start);
    sub[end - start] = '\0';

    return sub;
}

