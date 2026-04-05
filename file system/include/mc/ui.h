#ifndef MC_UI_H
#define MC_UI_H

#include <ncurses.h>

#include "mc/file_entry.h"

typedef struct {
    WINDOW *left;
    WINDOW *right;
    WINDOW *status;
} McWindows;

int mc_init_ui(McWindows *windows);
void mc_draw_panel(WINDOW *win, const FileEntry *files, int count, int highlight);
void mc_render_ui(
    const McWindows *windows,
    const FileEntry *left_files,
    int left_count,
    int left_index,
    const FileEntry *right_files,
    int right_count,
    int right_index,
    int active_panel
);
void mc_destroy_ui(McWindows *windows);

#endif
