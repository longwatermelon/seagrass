#ifndef FS_H
#define FS_H

char** fs_read_dir(const char* path, unsigned char type, int* count);

char* fs_parent(const char* file);
char* fs_filename(const char* file);

#endif

