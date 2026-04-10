#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread_shops.h"



void* loading(void* arg) {
    state_shop_t *shop_state = (state_shop_t*)arg;
    int truck_load = 0;
    int shop_index = 0;
    while (1) {
        // Проверяем условие остановки отдельно от работы с магазинами.
        pthread_mutex_lock(&shop_state->mutex_buyers_end);
        int buyers_done = shop_state->count_buyers_end;
        pthread_mutex_unlock(&shop_state->mutex_buyers_end);

        if (buyers_done >= 3) {
            break;
        }

        truck_load = rand() % 100 + 201;
        shop_index = rand() % 5;

        // Пополнение выполняется под мьютексом конкретного магазина.
        pthread_mutex_lock(&shop_state->mutex_shop[shop_index]);
        shop_state->shop[shop_index] += truck_load;
        printf("Trucker is loading %d items in shop %d.\n", truck_load, shop_index + 1);  
        pthread_mutex_unlock(&shop_state->mutex_shop[shop_index]);
        sleep(1);
      
    }

    return NULL;
}
