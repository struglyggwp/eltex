#ifndef EVENTS_H
#define EVENTS_H

#include "common.h"

struct ui_event {
    enum event_type type;
    char text[MAX_MESSAGE_LEN];
    char username[MAX_NAME_LEN];
};

int events_push(enum event_type type, const char *text, const char *username);
int events_pop(struct ui_event *event);

#endif // EVENTS_H
