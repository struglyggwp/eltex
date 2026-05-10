#ifndef UI_NCURSES_H
#define UI_NCURSES_H

#include <stddef.h>

/* Непрозрачная структура интерфейса. Ее поля доступны только внутри ui.c. */
typedef struct Ui Ui;

typedef enum {
    UI_KEY_NONE = 0,
    /* Пользователь нажал Enter, готовое сообщение лежит в submitted. */
    UI_KEY_SUBMIT,
    /* Пользователь нажал Esc, можно завершать главный цикл. */
    UI_KEY_QUIT,
    /* Терминал изменил размер, интерфейс уже перерисован. */
    UI_KEY_RESIZE
} UiEvent;

/* Запускает ncurses и создает интерфейс. */
int ui_init(Ui **ui);

/* Завершает ncurses и освобождает ресурсы интерфейса. */
void ui_destroy(Ui *ui);

/* Перерисовывает весь интерфейс. */
void ui_draw(Ui *ui);

/* Добавляет сообщение в левую область сообщений. */
int ui_add_message(Ui *ui, const char *message);

/* Добавляет имя в правую область пользователей. */
int ui_add_user(Ui *ui, const char *name);

/* Удаляет имя из правой области пользователей. */
int ui_remove_user(Ui *ui, const char *name);

/* Очищает левую область сообщений. */
void ui_clear_messages(Ui *ui);

/* Очищает правую область пользователей. */
void ui_clear_users(Ui *ui);

/* Обрабатывает одну клавишу и возвращает событие для внешней логики. */
UiEvent ui_handle_input(Ui *ui, char *submitted, size_t submitted_size);

#endif
