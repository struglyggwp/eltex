#include <ncurses.h>

#include "mc/input.h"

int mc_handle_input( 
    int ch,
    int *active_panel,
    int *left_index,
    int left_count,
    int *right_index,
    int right_count
) {
    if (ch == 'q') {
        return 1;
    }

    if (*active_panel == 0) {
        switch (ch) {
            case KEY_UP:
                if (*left_index > 0) {
                    --(*left_index);
                }
                break;
            case KEY_DOWN:
                if (*left_index < left_count - 1) {
                    ++(*left_index);
                }
                break;
            case '\t':
                *active_panel = 1;
                break;
        }
    } else {
        switch (ch) {
            case KEY_UP:
                if (*right_index > 0) {
                    --(*right_index);
                }
                break;
            case KEY_DOWN:
                if (*right_index < right_count - 1) {
                    ++(*right_index);
                }
                break;
            case '\t':
                *active_panel = 0;
                break;
        }
    }

    return 0;
}
