#define _XOPEN_SOURCE 700

#include "ui.h"
#include "common.h"

#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

struct Ui {
    WINDOW *messages_win;
    WINDOW *users_win;
    WINDOW *input_win;

    int screen_height;
    int screen_width;

    char messages[MAX_MESSAGES][MAX_MESSAGE_LEN];
    size_t messages_count;
    size_t messages_scroll;

    char users[MAX_USERS][MAX_NAME_LEN];
    size_t users_count;

    wchar_t input[MAX_INPUT_LEN];
    size_t input_len;
};

/* Безопасно копирует строку в буфер и всегда добавляет '\0'. */
static void ui_copy_text(char *dst, size_t dst_size, const char *src)
{
    if (dst_size == 0) {
        return;
    }

    if (src == NULL) {
        dst[0] = '\0';
        return;
    }

    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

/* Возвращает видимую длину строки, чтобы UTF-8 текст центрировался нормально. */
static int ui_text_width(const char *text)
{
    size_t width = mbstowcs(NULL, text, 0);

    if (width == (size_t)-1) {
        return (int)strlen(text);
    }

    return (int)width;
}

/* Считает ширину wide-строки в клетках терминала. */
static int ui_wtext_width(const wchar_t *text, size_t len)
{
    int width = 0;

    for (size_t i = 0; i < len; ++i) {
        int symbol_width = wcwidth(text[i]);

        if (symbol_width > 0) {
            width += symbol_width;
        }
    }

    return width;
}

/* Копирует wide-строку в обычный UTF-8 буфер для внешней логики. */
static void ui_wide_to_text(char *dst, size_t dst_size, const wchar_t *src)
{
    size_t converted;

    if (dst == NULL || dst_size == 0) {
        return;
    }

    converted = wcstombs(dst, src, dst_size - 1);
    if (converted == (size_t)-1) {
        dst[0] = '\0';
        return;
    }

    dst[converted] = '\0';
}

/* Создает или пересоздает три ncurses-окна под текущий размер терминала. */
static void ui_recreate_windows(Ui *ui)
{
    int height;
    int width;
    int input_height = 3;
    int gap = 1;
    int users_width;
    int messages_width;
    int top_height;

    getmaxyx(stdscr, height, width);
    ui->screen_height = height;
    ui->screen_width = width;

    if (ui->messages_win != NULL) {
        delwin(ui->messages_win);
        ui->messages_win = NULL;
    }
    if (ui->users_win != NULL) {
        delwin(ui->users_win);
        ui->users_win = NULL;
    }
    if (ui->input_win != NULL) {
        delwin(ui->input_win);
        ui->input_win = NULL;
    }

    if (height < 8 || width < 32) {
        return;
    }

    /* Правая колонка занимает примерно четверть экрана, но остается читаемой. */
    users_width = width / 4;
    if (users_width < 18) {
        users_width = 18;
    }
    if (users_width > 28) {
        users_width = 28;
    }

    messages_width = width - users_width - gap;
    top_height = height - input_height - gap;

    ui->messages_win = newwin(top_height, messages_width, 0, 0);
    ui->users_win = newwin(top_height, users_width, 0, messages_width + gap);
    ui->input_win = newwin(input_height, width, top_height + gap, 0);

    keypad(ui->input_win, TRUE);
    nodelay(ui->input_win, TRUE);
}

/* Печатает текст по центру выбранной строки внутри окна. */
static void ui_draw_centered(WINDOW *win, int row, const char *text)
{
    int height;
    int width;
    int col;

    getmaxyx(win, height, width);
    (void)height;

    col = (width - ui_text_width(text)) / 2;
    if (col < 1) {
        col = 1;
    }

    mvwprintw(win, row, col, "%s", text);
}

/* Очищает и перерисовывает левое окно с сообщениями пользователей. */
static void ui_draw_messages(Ui *ui)
{
    int height;
    int width;
    int row;
    size_t first;
    size_t visible_count;

    if (ui->messages_win == NULL) {
        return;
    }

    werase(ui->messages_win);
    box(ui->messages_win, 0, 0);
    getmaxyx(ui->messages_win, height, width);

    if (ui->messages_count == 0) {
        ui_draw_centered(ui->messages_win, height / 2, "Сообщения пользователей");
    } else {
        visible_count = (size_t)(height - 2);
        first = 0;
        if (ui->messages_count > visible_count) {
            first = ui->messages_count - visible_count;
        }

        row = 1;
        for (size_t i = first; i < ui->messages_count && row < height - 1; ++i) {
            mvwprintw(ui->messages_win, row, 1, "%.*s",
                      width - 2, ui->messages[i]);
            ++row;
        }
    }

    wnoutrefresh(ui->messages_win);
}

/* Очищает и перерисовывает правое окно со списком пользователей. */
static void ui_draw_users(Ui *ui)
{
    int height;
    int width;
    int row;
    int start_row;

    if (ui->users_win == NULL) {
        return;
    }

    werase(ui->users_win);
    box(ui->users_win, 0, 0);
    getmaxyx(ui->users_win, height, width);

    start_row = 1;
    for (size_t i = 0; i < ui->users_count && start_row < height / 2 - 2; ++i) {
        mvwprintw(ui->users_win, start_row, 1, "%.*s", width - 2, ui->users[i]);
        ++start_row;
    }

    row = height / 2 + 3;
    for (size_t i = (size_t)start_row - 1; i < ui->users_count && row < height - 1; ++i) {
        mvwprintw(ui->users_win, row, 1, "%.*s", width - 2, ui->users[i]);
        ++row;
    }

    wnoutrefresh(ui->users_win);
}

/* Очищает и перерисовывает нижнее поле ввода. */
static void ui_draw_input(Ui *ui)
{
    int height;
    int width;
    int input_width;
    int cursor_col;
    size_t first_symbol;

    if (ui->input_win == NULL) {
        return;
    }

    werase(ui->input_win);
    box(ui->input_win, 0, 0);
    getmaxyx(ui->input_win, height, width);
    (void)height;

    if (ui->input_len == 0) {
        ui_draw_centered(ui->input_win, 1, "");
    } else {
        input_width = width - 2;
        first_symbol = 0;

        while (first_symbol < ui->input_len &&
               ui_wtext_width(ui->input + first_symbol,
                              ui->input_len - first_symbol) > input_width) {
            first_symbol++;
        }

        mvwaddwstr(ui->input_win, 1, 1, ui->input + first_symbol);

        cursor_col = 1 + ui_wtext_width(ui->input + first_symbol,
                                        ui->input_len - first_symbol);
        if (cursor_col > width - 2) {
            cursor_col = width - 2;
        }
        wmove(ui->input_win, 1, cursor_col);
    }

    wnoutrefresh(ui->input_win);
}

int ui_init(Ui **ui)
{
    Ui *created;

    if (ui == NULL) {
        return -1;
    }

    created = calloc(1, sizeof(*created));
    if (created == NULL) {
        return -1;
    }

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(1);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        use_default_colors();
    }

    *ui = created;
    ui_recreate_windows(created);
    ui_draw(created);

    return 0;
}

void ui_destroy(Ui *ui)
{
    if (ui == NULL) {
        return;
    }

    if (ui->messages_win != NULL) {
        delwin(ui->messages_win);
    }
    if (ui->users_win != NULL) {
        delwin(ui->users_win);
    }
    if (ui->input_win != NULL) {
        delwin(ui->input_win);
    }

    endwin();
    free(ui);
}

void ui_draw(Ui *ui)
{
    int height;
    int width;

    if (ui == NULL) {
        return;
    }

    getmaxyx(stdscr, height, width);
    if (height != ui->screen_height ||
        width != ui->screen_width ||
        ui->messages_win == NULL ||
        ui->users_win == NULL ||
        ui->input_win == NULL) {
        ui_recreate_windows(ui);
    }

    erase();

    if (ui->messages_win == NULL ||
        ui->users_win == NULL ||
        ui->input_win == NULL) {
        mvprintw(0, 0, "Terminal is too small");
        refresh();
        return;
    }

    ui_draw_messages(ui);
    ui_draw_users(ui);
    ui_draw_input(ui);
    doupdate();
}

/* Добавляет сообщение в историю и обновляет окно сообщений. */
int ui_add_message(Ui *ui, const char *message)
{
    if (ui == NULL || message == NULL) {
        return -1;
    }

    /* Если буфер сообщений заполнен, удаляем самое старое сообщение. */
    if (ui->messages_count == MAX_MESSAGES) {
        memmove(ui->messages, ui->messages + 1,
                (MAX_MESSAGES - 1) * sizeof(ui->messages[0]));
        ui->messages_count--;
    }

    ui_copy_text(ui->messages[ui->messages_count],
                 sizeof(ui->messages[ui->messages_count]), message);
    ui->messages_count++;
    ui_draw(ui);

    return 0;
}

/* Добавляет пользователя в список, если такого имени еще нет. */
int ui_add_user(Ui *ui, const char *name)
{
    if (ui == NULL || name == NULL || ui->users_count == MAX_USERS) {
        return -1;
    }

    for (size_t i = 0; i < ui->users_count; ++i) {
        if (strcmp(ui->users[i], name) == 0) {
            return 0;
        }
    }

    ui_copy_text(ui->users[ui->users_count],
                 sizeof(ui->users[ui->users_count]), name);
    ui->users_count++;
    ui_draw(ui);

    return 0;
}

/* Удаляет пользователя по имени и перерисовывает список. */
int ui_remove_user(Ui *ui, const char *name)
{
    if (ui == NULL || name == NULL) {
        return -1;
    }

    for (size_t i = 0; i < ui->users_count; ++i) {
        if (strcmp(ui->users[i], name) == 0) {
            memmove(ui->users + i, ui->users + i + 1,
                    (ui->users_count - i - 1) * sizeof(ui->users[0]));
            ui->users_count--;
            ui_draw(ui);
            return 0;
        }
    }

    return -1;
}

/* Очищает список сообщений на экране. */
void ui_clear_messages(Ui *ui)
{
    if (ui == NULL) {
        return;
    }

    ui->messages_count = 0;
    ui_draw(ui);
}

/* Очищает список пользователей на экране. */
void ui_clear_users(Ui *ui)
{
    if (ui == NULL) {
        return;
    }

    ui->users_count = 0;
    ui_draw(ui);
}

/* Читает одну клавишу и обновляет поле ввода. Enter отдает готовую строку наружу. */
UiEvent ui_handle_input(Ui *ui, char *submitted, size_t submitted_size)
{
    int result;
    int height;
    int width;
    wint_t ch;

    if (ui == NULL) {
        return UI_KEY_NONE;
    }

    getmaxyx(stdscr, height, width);
    if (height != ui->screen_height || width != ui->screen_width) {
        ui_recreate_windows(ui);
        ui_draw(ui);
        return UI_KEY_RESIZE;
    }

    if (ui->input_win == NULL) {
        ui_draw(ui);
        return UI_KEY_NONE;
    }

    result = wget_wch(ui->input_win, &ch);
    if (result == ERR) {
        return UI_KEY_NONE;
    }

    /* При изменении размера терминала пересобираем окна под новую геометрию. */
    if (result == KEY_CODE_YES && ch == KEY_RESIZE) {
        endwin();
        refresh();
        clear();
        ui_recreate_windows(ui);
        ui_draw(ui);
        return UI_KEY_RESIZE;
    }

    if (ch == 27) {
        return UI_KEY_QUIT;
    }

    /* Enter: копируем введенное сообщение вызывающему коду и очищаем поле ввода. */
    if (ch == '\n' || ch == '\r') {
        if (submitted != NULL && submitted_size > 0) {
            ui_wide_to_text(submitted, submitted_size, ui->input);
        }

        ui->input[0] = L'\0';
        ui->input_len = 0;
        ui_draw(ui);
        return UI_KEY_SUBMIT;
    }

    /* Backspace удаляет последний введенный символ целиком. */
    if ((result == KEY_CODE_YES && ch == KEY_BACKSPACE) || ch == 127 || ch == '\b') {
        if (ui->input_len > 0) {
            ui->input_len--;
            ui->input[ui->input_len] = L'\0';
            ui_draw(ui);
        }
        return UI_KEY_NONE;
    }

    /* Обычные печатные символы дописываем в буфер ввода. */
    if (result == OK && iswprint(ch) && ui->input_len < MAX_INPUT_LEN - 1) {
        ui->input[ui->input_len] = (wchar_t)ch;
        ui->input_len++;
        ui->input[ui->input_len] = L'\0';
        ui_draw(ui);
    }

    return UI_KEY_NONE;
}
