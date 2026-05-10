#include "client.h"
#include "events.h"

#include <errno.h>
#include <sys/msg.h>
#include <time.h>

void* receive(void *arg){

    struct client_context *ctx = arg;
 

    while(!ctx->should_exit){
    
        struct msg_buffer msg;
        if (msgrcv(ctx->msgid, &msg, sizeof(msg) - sizeof(msg.mtype),
                ctx->client_id, IPC_NOWAIT) == -1) {
            if (errno == ENOMSG) {
                nanosleep(&(struct timespec){.tv_nsec = 10000000}, NULL);
                continue;
            }
            break;
        }

            switch(msg.msg_type_) {
                case EVENT_CHAT_MESSAGE: events_push(EVENT_CHAT_MESSAGE, msg.mtext, msg.username); break;
                case EVENT_USER_JOINED: events_push(EVENT_USER_JOINED, NULL, msg.username); break;
                case EVENT_USER_LEFT: events_push(EVENT_USER_LEFT, NULL, msg.username); break;
                default: break;
            }
        
        
    
       
    }
    return NULL;
}
