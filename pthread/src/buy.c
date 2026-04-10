#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread_shops.h"



void* buy(void* arg) {
    state_shop_t *shop_state = (state_shop_t*)arg;
    int demand = rand() % 200 + 9901;
    printf("I'm a customer number: %ld. I need  %d items.\n", (long)pthread_self(), demand);
    int shop_index, count_items = 0;
    
    while(demand > 0){

            count_items = 0;
            shop_index = rand() % 5;

            // Покупатель работает только с одним магазином за итерацию,
            // поэтому блокирует только его мьютекс.
            pthread_mutex_lock(&shop_state->mutex_shop[shop_index]);
                printf("Customer %ld is trying to buy from shop %d.\n", (long)pthread_self(), shop_index + 1);

                // Забираем товар, пока в магазине есть остаток и спрос не закрыт.
                while(shop_state->shop[shop_index] > 0 && demand > 0){
                    shop_state->shop[shop_index]--;
                    demand--;
                    count_items++;
                }
                if (count_items > 0){ 
                    printf("Customer %ld bought %d items from shop %d. Remaining demand: %d\n", (long)pthread_self(), count_items, shop_index + 1, demand);
                    pthread_mutex_unlock(&shop_state->mutex_shop[shop_index]);
                    sleep(3);
                }
                else{
                    pthread_mutex_unlock(&shop_state->mutex_shop[shop_index]);
                    sleep(3);
                }
            
           
    }

    printf("Customer %ld has satisfied their demand.\n", (long)pthread_self());

    // Увеличиваем общий счётчик под отдельным мьютексом, чтобы грузчик
    // корректно увидел завершение всех покупателей.
    pthread_mutex_lock(&shop_state->mutex_buyers_end);
    shop_state->count_buyers_end++;
    pthread_mutex_unlock(&shop_state->mutex_buyers_end);

    return NULL;
}
