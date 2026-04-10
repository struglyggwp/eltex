#include <stdlib.h>
#include <time.h>
#include "pthread_shops.h"


int run() {

   
    state_shop_t shop_state;

    pthread_mutex_init(&shop_state.mutex_buyers_end, NULL);

    shop_state.count_buyers_end = 0;

    srand(time(NULL));

    for(int i = 0; i < 5; i++) {
        pthread_mutex_init(&shop_state.mutex_shop[i], NULL);
    }

    for(int i = 0; i < 5; i++) {
        shop_state.shop[i] = rand() % 201 + 900; 
       
    }

    printshops(&shop_state);
    running(&shop_state);

    // Освобождаем синхронизационные примитивы после завершения всех потоков.
    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&shop_state.mutex_shop[i]);
    }
    pthread_mutex_destroy(&shop_state.mutex_buyers_end);



    return 0;
}
