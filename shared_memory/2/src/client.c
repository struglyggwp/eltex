#include "client.h"
#include "events.h"
#include "ui.h"

#include <pthread.h>
#include <signal.h>
#include <stdio.h>

static volatile sig_atomic_t should_exit = 0;

static void handle_exit_signal(int signal)
{
    (void)signal;
    should_exit = 1;
}



void* client_init(void *arg) {

    struct client_context *ctx = arg;

    Ui *Ui;
    char message[MAX_INPUT_LEN];

    signal(SIGINT, handle_exit_signal);
    signal(SIGTERM, handle_exit_signal);



    if (ui_init(&Ui) != 0) {
        fprintf(stderr, "Ошибка инициализации интерфейса\n");
        return NULL;
    }


    while (!should_exit && !ctx->should_exit) {
        UiEvent event = ui_handle_input(Ui, message, sizeof(message));
    
        if (event == UI_KEY_QUIT) {
            ctx->should_exit = 1;
            break;
        }

        if (event == UI_KEY_SUBMIT && message[0] != '\0') {
            sender(message, ctx);
        }

        struct ui_event app_event;

        while (events_pop(&app_event)) {
            if (app_event.type == EVENT_CHAT_MESSAGE) {
            char line[MAX_MESSAGE_LEN + MAX_NAME_LEN + 3];
            snprintf(line, sizeof(line), "%s: %s", app_event.username, app_event.text);
            ui_add_message(Ui, line);
            }

            if (app_event.type == EVENT_USER_JOINED) {
            ui_add_user(Ui, app_event.username);
            }

            if (app_event.type == EVENT_USER_LEFT) {
            ui_remove_user(Ui, app_event.username);
            }

            if (app_event.type == EVENT_SYSTEM_MESSAGE) {
            ui_add_message(Ui, app_event.text);
            }
        }

    }
    
    pthread_mutex_lock(&ctx->mutex);
    ctx->should_exit = 1;
    pthread_mutex_unlock(&ctx->mutex);
    ui_destroy(Ui);
    return NULL;
}
