#ifndef COMMON_H
#define COMMON_H

#define MAX_MESSAGES 128
#define MAX_USERS 64
#define MAX_MESSAGE_LEN 256
#define MAX_NAME_LEN 64
#define MAX_INPUT_LEN 256

#define CLIENT_TO_SERVER 2

enum event_type {
    EVENT_CHAT_MESSAGE, // новое сообщение в чате
    EVENT_USER_JOINED, // новый пользователь подключился
    EVENT_USER_LEFT, // пользователь отключился
    EVENT_SYSTEM_MESSAGE // системное сообщение 
};

struct msg_buffer {
    long mtype;
    long sender_id;
    char mtext[MAX_MESSAGE_LEN];
    char username[MAX_NAME_LEN];
    enum event_type msg_type_; // 1 - chat message, 2 - user joined, 3 - user left
};

#endif // COMMON_H