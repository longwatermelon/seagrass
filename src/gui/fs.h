#ifndef FS_H
#define FS_H

#include <stdbool.h>

char** fs_read_dir(const char* path, unsigned char type, int* count);

char* fs_parent(const char* file);
char* fs_filename(const char* file);

bool fs_is_binary(const char* text);

char* fs_read_file(const char* path);

#endif

