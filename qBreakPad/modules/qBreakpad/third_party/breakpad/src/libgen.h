#ifndef LIBGEN_WINDOWS_H
#define LIBGEN_WINDOWS_H

#include <string.h>
#include <stdlib.h>

char* basename(char* path) {
    char* base = strrchr(path, '\\');
    return base ? base + 1 : path;
}

char* dirname(char* path) {
    static char dir[256];
    strcpy(dir, path);
    char* last_slash = strrchr(dir, '\\');
    if (last_slash) {
        *last_slash = '\0';
    } else {
        strcpy(dir, ".");
    }
    return dir;
}

#endif // LIBGEN_WINDOWS_H
