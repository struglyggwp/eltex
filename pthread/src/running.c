#include <stdlib.h>
#include <stdio.h>
#include "pthread_shops.h"





int running(state_shop_t *shop_state) {
    pthread_t buyers[3], trucker;
    int rc;
    // Грузчик работает параллельно с покупателями и пополняет случайные магазины.
    rc = pthread_create(&trucker, NULL, loading, shop_state);
    if (rc != 0) {
        fprintf(stderr, "Error creating trucker thread\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        rc = pthread_create(&buyers[i], NULL, buy, shop_state);
        if (rc != 0) {
            fprintf(stderr, "Error creating buyer thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Сначала дожидаемся всех покупателей, затем грузчик сам завершится по условию.
    for (int i = 0; i < 3; i++) {
        pthread_join(buyers[i], NULL);
    }

    pthread_join(trucker, NULL);
    printf("All buyers have satisfied their demand. Ending the program.\n");


    return 0;

}
