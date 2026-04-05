#include <ncurses.h>

#include "mc/ui.h"

void mc_destroy_ui(McWindows *windows) { // 
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
}
