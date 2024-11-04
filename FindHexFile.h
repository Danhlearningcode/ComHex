#ifndef _TEST_FIND_HEX_H
#define _TEST_FIND_HEX_H
#include <windows.h>
#include <stdio.h>
#include "string.h"
#include "stdlib.h"

typedef struct {
    char name[256]; 
    int level; 
    int is_dir; 
    char full_path[1024];
}DirectoryEntry;

int is_direction(const char *path);
int has_hex(const char *fileName);
int Read_Direction(const char *dir_name, int level, DirectoryEntry *Entries, int *entry_count);

#endif // _TEST_FIND_HEX_H