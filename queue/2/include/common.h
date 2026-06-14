#ifndef COMMON_H
#define COMMON_H

#define MAX_MESSAGES 128
#define MAX_USERS 64
#define MAX_MESSAGE_LEN 256
#define MAX_NAME_LEN 64
#define MAX_INPUT_LEN 256

#define CLIENT_TO_SERVER 2

enum event_type {
    EVENT_CHAT_MESSAGE, 
    EVENT_USER_JOINED, 
    EVENT_USER_LEFT, 
    EVENT_SYSTEM_MESSAGE 
};

struct msg_buffer {
    long mtype;
    long sender_id;
    char mtext[MAX_MESSAGE_LEN];
    char username[MAX_NAME_LEN];
    enum event_type msg_type_; 
};

#endif // COMMON_H