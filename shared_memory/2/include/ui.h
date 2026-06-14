#ifndef UI_NCURSES_H
#define UI_NCURSES_H

#include <stddef.h>

typedef struct Ui Ui;

typedef enum {
    UI_KEY_NONE = 0,
    UI_KEY_SUBMIT,
    UI_KEY_QUIT,
    UI_KEY_RESIZE
} UiEvent;

int ui_init(Ui **ui);

void ui_destroy(Ui *ui);

void ui_draw(Ui *ui);

int ui_add_message(Ui *ui, const char *message);

int ui_add_user(Ui *ui, const char *name);

int ui_remove_user(Ui *ui, const char *name);

void ui_clear_messages(Ui *ui);

void ui_clear_users(Ui *ui);

UiEvent ui_handle_input(Ui *ui, char *submitted, size_t submitted_size);

#endif
