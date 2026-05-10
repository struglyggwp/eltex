#ifndef SERVER_H
#define SERVER_H

#include "common.h"

struct client {
    long client_id;
    char name[MAX_NAME_LEN];
};

#endif // SERVER_H
