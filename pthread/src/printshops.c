#include <stdio.h>
#include "pthread_shops.h"

void printshops(state_shop_t *shop_state) {
    printf("Current shop inventories:\n");
    for (int i = 0; i < 5; i++) {
        printf("Shop %d: %d items\n", i + 1, shop_state->shop[i]);
    }
}
