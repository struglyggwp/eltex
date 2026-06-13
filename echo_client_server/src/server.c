#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>
#include "inet_proto.h"


static volatile sig_atomic_t run = 1;

void handle_sigint(int signo)
{
    (void)signo;
    run = 0;
}


int main(void)
{

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction SIGINT");
        exit(EXIT_FAILURE);
    }
    
    if(sigaction(SIGTERM, &sa, NULL) == -1){
        perror("error sigaction");
        exit(EXIT_FAILURE);
    }

    struct client_info *client_head = NULL;

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

    while (run) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
    
        if (parse_packet(buffer, bytes, PORT) == -1) continue;

        struct iphdr *iph = (struct iphdr *)buffer;
        int ip_header_len = iph->ihl * 4;
        struct udphdr *udph = (struct udphdr *)(buffer + ip_header_len);
        int payload_len = ntohs(udph->len) - sizeof(struct udphdr);
        char *payload = buffer + ip_header_len + sizeof(struct udphdr);

        uint32_t client_ip = iph->saddr;
        uint16_t client_port = ntohs(udph->source);

        struct client_info *client = find_client(client_head, client_ip, client_port);

        if (client == NULL) {

            if (clients_count >= MAX_CLIENTS) {
                printf("[Сервер] Слишком много клиентов\n");
                continue;
            }

            client = add_client(&client_head, client_ip, client_port);

            if (client == NULL) continue;

            clients_count++;

        }

        if (payload_len == 4 && memcmp(payload, "exit", 4) == 0) {
            
            printf("[Сервер] Клиент завершил соединение\n");
            remove_client(&client_head, client);
            clients_count--;

            continue;
        }

        client->count++;

        printf("[Сервер] Получено: %.*s\n", payload_len, payload);

        char answer[256];

        int answer_len = snprintf(answer, sizeof(answer), "%.*s %d", payload_len, payload, client->count);

        printf("[Сервер] Отправляю echo-ответ: %s\n", answer);

        char send_buf[BUFFER_SIZE];
        memset(send_buf, 0, sizeof(send_buf));

        struct iphdr *ip_reply = (struct iphdr *)send_buf;
        struct udphdr *udp_reply = (struct udphdr *)(send_buf + sizeof(struct iphdr));
        char *reply_data = send_buf + sizeof(struct iphdr) + sizeof(struct udphdr);

        memcpy(reply_data, answer, answer_len);

        int packet_size = sizeof(struct iphdr) + sizeof(struct udphdr) + answer_len;

        ipv4_head_push(ip_reply, "127.0.0.1", "127.0.0.1", IPPROTO_UDP, answer_len);
        udp_head_push(udp_reply, client_port, PORT, answer_len);

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

    printf("Завершение работы\n");
    free_clients(client_head);
    close(fd);
    return 0;
}