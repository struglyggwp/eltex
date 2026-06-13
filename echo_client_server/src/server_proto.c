#include "inet_proto.h"
#include <stdio.h>



struct client_info *find_client(struct client_info *head, uint32_t ip_client, uint16_t port_client)
{
    struct client_info *current = head;

    while (current != NULL) {
        if (current->ip_client == ip_client &&
            current->port_client == port_client) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

struct client_info *add_client(struct client_info **head, uint32_t ip_client, uint16_t port_client)
{
    struct client_info *new_client = calloc(1, sizeof(struct client_info));
    if (new_client == NULL) {
        perror("calloc");
        return NULL;
    }

    new_client->ip_client = ip_client;
    new_client->port_client = port_client;
    new_client->count = 0;

    new_client->prev = NULL;
    new_client->next = *head;

    if (*head != NULL) {
        (*head)->prev = new_client;
    }

    *head = new_client;

    return new_client;
}

void remove_client(struct client_info **head, struct client_info *client)
{
    if (client == NULL) {
        return;
    }

    if (client->prev != NULL) {
        client->prev->next = client->next;
    } else {
        *head = client->next;
    }

    if (client->next != NULL) {
        client->next->prev = client->prev;
    }

    free(client);
}

void free_clients(struct client_info *head)
{
    struct client_info *current = head;

    while (current != NULL) {
        struct client_info *next = current->next;
        free(current);
        current = next;
    }
}
