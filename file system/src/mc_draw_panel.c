#include <ncurses.h>

#include "mc/ui.h"

void mc_draw_panel(WINDOW *win, const FileEntry *files, int count, int highlight) {
    werase(win); // Очищаем окно перед отрисовкой
    box(win, 0, 0); // Рисуем рамку вокруг окна

    for (int i = 0; i < count; ++i) { // Проходим по всем файлам и директориям
        if (i == highlight) {
            wattron(win, A_REVERSE);
        }

        mvwprintw(win, i + 1, 1, "%s %s", files[i].name, files[i].type); // Выводим имя и тип файла/директории

        if (i == highlight) {
            wattroff(win, A_REVERSE); 
        }
    }

    wrefresh(win); 
}
