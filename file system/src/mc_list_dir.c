#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "mc/constants.h"
#include "mc/file_ops.h"

int mc_list_dir(const char *path, FileEntry *files, int *count) {
    DIR *dir = opendir(path);
    char full_path[MC_PATH_BUFFER_SIZE];
    struct stat st;

    if (dir == NULL) {
        perror("Ошибка открытия директории");
        *count = 0;
        return -1;
    }

    *count = 0;

    for (struct dirent *entry = readdir(dir);
         entry != NULL && *count < MC_MAX_FILES;
         entry = readdir(dir)) {
        if (strcmp(entry->d_name, ".") == 0) continue; // Пропускаем текущую директорию
        files[*count].name = mc_xstrdup(entry->d_name);
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) { // Проверяем тип файла
        files[*count].type = "[DIR]";
        } else {
         files[*count].type = "[FILE]";
        }
        if (files[*count].name == NULL) {
            fprintf(stderr, "Ошибка выделения памяти для файла %s\n", entry->d_name);
            mc_free_files(files, *count);
            closedir(dir);
            *count = 0;
            return -1;
        }

        ++(*count);
    }

    closedir(dir);
    return 0;
}
