#include <ncurses.h>

#include "mc/ui.h"

void mc_render_ui(
    const McWindows *windows,
    const FileEntry *left_files,
    int left_count,
    int left_index,
    const FileEntry *right_files,
    int right_count,
    int right_index,
    int active_panel
) {
    mc_draw_panel(windows->left, left_files, left_count, left_index);
    mc_draw_panel(windows->right, right_files, right_count, right_index);

    werase(windows->status);
    mvwprintw(
        windows->status,
        0,
        0,
        "TAB - switch panel, arrows - navigation, Q - exit, active panel: %s",
        active_panel == 0 ? "left" : "right"
    );
    wrefresh(windows->status);
}
