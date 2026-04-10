#ifndef PTHREAD_SHOPS_H
#define PTHREAD_SHOPS_H

#include <pthread.h>


// Общее состояние модели, доступное всем рабочим потокам.
typedef struct {
    // Защищает счётчик завершившихся покупателей.
    pthread_mutex_t mutex_buyers_end;
    // Отдельный мьютекс на каждый магазин, чтобы потоки не блокировали друг друга глобально.
    pthread_mutex_t mutex_shop[5];
    // Текущие остатки товаров по магазинам.
    int shop[5];
    // Количество покупателей, полностью закрывших свой спрос.
    int count_buyers_end;
} state_shop_t;



void printshops(state_shop_t *shop_state);
void* buy(void* arg);
void* loading(void* arg);
int running(state_shop_t *shop_state);
int run();


#endif // PTHREAD_SHOPS_H
