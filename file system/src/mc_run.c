#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "mc/app.h"
#include "mc/constants.h"
#include "mc/file_entry.h"
#include "mc/file_ops.h"
#include "mc/input.h"
#include "mc/ui.h"



int mc_run(void) {
    struct stat st; // структура для получения информации о файлах
    char cwd[MC_PATH_BUFFER_SIZE];  // буфер для хранения текущей директории
    FileEntry left_files[MC_MAX_FILES] = {0};   // массив для файлов левой панели
    FileEntry right_files[MC_MAX_FILES] = {0};  // массив для файлов правой панели
    char left_path[MC_PATH_BUFFER_SIZE] = {0};
    char right_path[MC_PATH_BUFFER_SIZE] = {0};
    char left_active_path[MC_PATH_BUFFER_SIZE] = {0}; // буфер для хранения пути к выделенному файлу в левой панели
    char right_active_path[MC_PATH_BUFFER_SIZE] = {0}; // буфер для хранения пути к выделенному файлу в правой панели
    int left_count = 0;     // количество файлов в левой панели
    int right_count = 0;    // количество файлов в правой панели
    int left_index = 0;     // индекс выделенного файла в левой панели
    int right_index = 0;    // индекс выделенного файла в правой панели
    int active_panel = 0;   // Определяет какая панель активна 0 - левая панель, 1 - правая панель
    int ch;
    McWindows windows = {0};    // Окна для левой панели, правой панели и строки состояния


    if (getcwd(cwd, sizeof(cwd)) == NULL) {   // запись текущую директорию в cwd
        perror("Не удалось получить текущую директорию");
        return EXIT_FAILURE;
    }

    snprintf(left_path, sizeof(left_path), "%s", cwd);
    snprintf(right_path, sizeof(right_path), "%s", cwd);

    if (mc_list_dir(cwd, left_files, &left_count) == -1) {
        return EXIT_FAILURE;
    }

    if (mc_list_dir(cwd, right_files, &right_count) == -1) {
        mc_free_files(left_files, left_count);
        return EXIT_FAILURE;
    }

    if (mc_init_ui(&windows) == -1) {
        mc_free_files(left_files, left_count);
        mc_free_files(right_files, right_count);
        fprintf(stderr, "Не удалось инициализировать ncurses-интерфейс\n");
        return EXIT_FAILURE;
    }

    mc_render_ui(
        &windows,
        left_files,
        left_count,
        left_index,
        right_files,
        right_count,
        right_index,
        active_panel
    );

    while ((ch = getch()) != ERR) {
        if (mc_handle_input(
                ch,
                &active_panel,
                &left_index,
                left_count,
                &right_index,
                right_count)) {
            break;
        }


        if(ch == '\n' || ch == KEY_ENTER) {
            if(active_panel == 0) {
                if(left_files[left_index].name == NULL) {
                    continue; 
                }
                snprintf(left_active_path, sizeof(left_active_path), "%s/%s", left_path, left_files[left_index].name);
                if(stat(left_active_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                    // Если это директория, обновляем список файлов
                    mc_free_files(left_files, left_count); // Освобождаем память от предыдущего списка файлов
                    if (mc_list_dir(left_active_path, left_files, &left_count) == -1) {
                        fprintf(stderr, "Не удалось открыть директорию %s\n", left_active_path);
                    } else {
                        snprintf(left_path, sizeof(left_path), "%s", left_active_path); // Обновляем текущий путь левой панели
                        mc_draw_panel(windows.left, left_files, left_count, left_index); // Перерисовываем панель с новым списком файлов
                        left_index = 0; // Сброс индекса выделения при переходе в новую директорию
                    }
                }
            } else {
                if(right_files[right_index].name == NULL) {
                    continue; 
                }
                snprintf(right_active_path, sizeof(right_active_path), "%s/%s", right_path, right_files[right_index].name);
                if(stat(right_active_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                    // Если это директория, обновляем список файлов
                     mc_free_files(right_files, right_count); // Освобождаем память от предыдущего списка файлов
                    if (mc_list_dir(right_active_path, right_files, &right_count) == -1) {
                        fprintf(stderr, "Не удалось открыть директорию %s\n", right_active_path);
                    } else {
                        snprintf(right_path, sizeof(right_path), "%s", right_active_path); // Обновляем текущий путь правой панели
                        mc_draw_panel(windows.right, right_files, right_count, right_index); // Перерисовываем панель с новым списком файлов
                        right_index = 0; // Сброс индекса выделения при переходе в новую директорию
                    }
                }
            }
        }

        mc_render_ui(
            &windows,
            left_files,
            left_count,
            left_index,
            right_files,
            right_count,
            right_index,
            active_panel
        );
    }

    mc_destroy_ui(&windows);
    mc_free_files(left_files, left_count);
    mc_free_files(right_files, right_count);

    return EXIT_SUCCESS;
}
