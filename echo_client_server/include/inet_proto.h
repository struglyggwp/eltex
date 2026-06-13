#ifndef INET_PROTO_H

#define INET_PROTO_H

#define _POSIX_C_SOURCE 200809L

#define PORT 7777
#define BUFFER_SIZE 512
#define MAX_CLIENTS 10


#include <stdint.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/in.h>	
#include <arpa/inet.h>

struct client_info {
    uint32_t ip_client;
    uint16_t port_client;
    int count;

    struct client_info *next;
    struct client_info *prev;
};

void ipv4_head_push(struct iphdr *iph, char *ip_dest, char *ip_src, int ip_proto, ssize_t payload_len);
ssize_t udp_head_push(struct udphdr *udph, uint16_t port_dest, uint16_t port_src, ssize_t payload_len);
int parse_packet(char *packet, ssize_t bytes, uint16_t port_dest);

struct client_info *find_client(struct client_info *head, uint32_t ip_client, uint16_t port_client);
struct client_info *add_client(struct client_info **head, uint32_t ip_client, uint16_t port_client);
void remove_client(struct client_info **head, struct client_info *client);
void free_clients(struct client_info *head);

#endif