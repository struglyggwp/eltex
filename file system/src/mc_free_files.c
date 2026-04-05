#include <stdlib.h>

#include "mc/file_ops.h"

void mc_free_files(FileEntry *files, int count) { // Освобождаем память, выделенную для массива файлов
    for (int i = 0; i < count; ++i) {
        free(files[i].name);
        files[i].name = NULL;
    }
}
