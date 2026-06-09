#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PORT 7777
#define BUFFER_SIZE 512
#define MAX_CLIENTS 10

struct clients {
    uint32_t ip_client;
    uint16_t port_client;
    int count;
};

int main(void)
{
    struct clients clients_info[MAX_CLIENTS];
    memset(clients_info, 0, sizeof(clients_info));

    int clients_count = 0;

    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    int flag = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) == -1) {
        perror("setsockopt error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("RAW UDP сервер запущен на порту %d...\n", PORT);

    while (1) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            continue;
        }

        struct iphdr *iph = (struct iphdr *)buffer;

        if (iph->protocol != IPPROTO_UDP) {
            continue;
        }

        int ip_header_len = iph->ihl * 4;

        if (bytes < ip_header_len + (int)sizeof(struct udphdr)) {
            continue;
        }

        struct udphdr *udph = (struct udphdr *)(buffer + ip_header_len);

        if (ntohs(udph->dest) != PORT) {
            continue;
        }

        int payload_len = ntohs(iph->tot_len) - ip_header_len - sizeof(struct udphdr);

        if (payload_len <= 0) {
            continue;
        }

        char *payload = buffer + ip_header_len + sizeof(struct udphdr);

        uint32_t client_ip = iph->saddr;
        uint16_t client_port = ntohs(udph->source);

        int index = -1;

        for (int i = 0; i < clients_count; i++) {
            if (clients_info[i].ip_client == client_ip &&
                clients_info[i].port_client == client_port) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            if (clients_count >= MAX_CLIENTS) {
                printf("[Сервер] Слишком много клиентов\n");
                continue;
            }

            index = clients_count;
            clients_info[index].ip_client = client_ip;
            clients_info[index].port_client = client_port;
            clients_info[index].count = 0;
            clients_count++;
        }

        if (payload_len == 4 && memcmp(payload, "exit", 4) == 0) {
            printf("[Сервер] Клиент завершил соединение\n");
            clients_info[index].count = 0;
            continue;
        }

        clients_info[index].count++;

        printf("[Сервер] Получено: %.*s\n", payload_len, payload);

        char answer[256];

        snprintf(answer, sizeof(answer),
                 "%.*s %d",
                 payload_len,
                 payload,
                 clients_info[index].count);

        printf("[Сервер] Отправляю echo-ответ: %s\n", answer);

        char send_buf[BUFFER_SIZE];
        memset(send_buf, 0, sizeof(send_buf));

        struct iphdr *ip_reply = (struct iphdr *)send_buf;
        struct udphdr *udp_reply = (struct udphdr *)(send_buf + sizeof(struct iphdr));
        char *reply_data = send_buf + sizeof(struct iphdr) + sizeof(struct udphdr);

        int answer_len = strlen(answer);

        memcpy(reply_data, answer, answer_len);

        int packet_size = sizeof(struct iphdr) + sizeof(struct udphdr) + answer_len;

        ip_reply->version = 4;
        ip_reply->ihl = 5;
        ip_reply->tos = 0;
        ip_reply->tot_len = htons(packet_size);
        ip_reply->id = 0;
        ip_reply->frag_off = 0;
        ip_reply->ttl = 64;
        ip_reply->protocol = IPPROTO_UDP;
        ip_reply->check = 0;
        ip_reply->saddr = iph->daddr;
        ip_reply->daddr = iph->saddr;

        udp_reply->source = htons(PORT);
        udp_reply->dest = udph->source;
        udp_reply->len = htons(sizeof(struct udphdr) + answer_len);
        udp_reply->check = 0;

        struct sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));

        client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = iph->saddr;
        client_addr.sin_port = udph->source;

        if (sendto(fd, send_buf, packet_size, 0,
                   (struct sockaddr *)&client_addr,
                   sizeof(client_addr)) == -1) {
            perror("sendto error");
        }
    }

    close(fd);
    return 0;
}