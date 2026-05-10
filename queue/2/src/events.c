#include "events.h"

#include <pthread.h>
#include <string.h>

#define EVENTS_MAX 64

static struct ui_event events[EVENTS_MAX];

static int head = 0;
static int tail = 0;
static int count = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int events_push(enum event_type type, const char *text, const char *username)
{
    pthread_mutex_lock(&mutex);

    if (count == EVENTS_MAX) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    events[tail].type = type;

    if (text != NULL) {
        strncpy(events[tail].text, text, MAX_MESSAGE_LEN - 1);
        events[tail].text[MAX_MESSAGE_LEN - 1] = '\0';
    } else {
        events[tail].text[0] = '\0';
    }

    if (username != NULL) {
        strncpy(events[tail].username, username, MAX_NAME_LEN - 1);
        events[tail].username[MAX_NAME_LEN - 1] = '\0';
    } else {
        events[tail].username[0] = '\0';
    }

    tail = (tail + 1) % EVENTS_MAX;
    count++;

    pthread_mutex_unlock(&mutex);
    return 0;
}

int events_pop(struct ui_event *event)
{
    if (event == NULL) {
        return 0;
    }

    pthread_mutex_lock(&mutex);

    if (count == 0) {
        pthread_mutex_unlock(&mutex);
        return 0;
    }

    *event = events[head];

    head = (head + 1) % EVENTS_MAX;
    count--;

    pthread_mutex_unlock(&mutex);
    return 1;
}
