#include <ncurses.h>

#include "mc/ui.h"

int mc_init_ui(McWindows *windows) {
    int height;
    int width;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    height = LINES - 2;
    width = COLS / 2;

    windows->left = newwin(height, width, 0, 0);
    windows->right = newwin(height, width, 0, width);
    windows->status = newwin(1, COLS, LINES - 1, 0);

    if (windows->left == NULL || windows->right == NULL || windows->status == NULL) {
        if (windows->left != NULL) {
            delwin(windows->left);
            windows->left = NULL;
        }

        if (windows->right != NULL) {
            delwin(windows->right);
            windows->right = NULL;
        }

        if (windows->status != NULL) {
            delwin(windows->status);
            windows->status = NULL;
        }

        endwin();
        return -1;
    }

    return 0;
}
