#include <stdlib.h>
#include <string.h>

char *mc_xstrdup(const char *src) {
    size_t len = strlen(src) + 1;
    char *copy = malloc(len);

    if (copy != NULL) {
        memcpy(copy, src, len);
    }

    return copy;
}
