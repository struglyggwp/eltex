#ifndef MC_FILE_OPS_H
#define MC_FILE_OPS_H

#include "mc/file_entry.h"

char *mc_xstrdup(const char *src);
int mc_list_dir(const char *path, FileEntry *files, int *count);
void mc_free_files(FileEntry *files, int count);

#endif
