#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>
#include "inet_proto.h"
#include <sys/time.h>
#include <errno.h>

#define SERVER_PORT 7777
#define CLIENT_PORT 8888


int main() {
   

    char message[256];

    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    if(fd == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    int flag = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) < 0) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    struct timeval timeout;

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt SO_RCVTIMEO");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char packet[512];

    memset(packet, 0, 512);

    struct iphdr *iph = (struct iphdr*) packet;
    struct udphdr *udph = (struct udphdr*)(packet + sizeof(struct iphdr));
    char *data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 0;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int packet_size = 0;

    while(1){
    printf("Напишите echo-сообщение\n");
    
    memset(message, 0, sizeof(message));

    if (fgets(message, sizeof(message), stdin) == NULL) {
        perror("fgets error");
        continue;
    }

    message[strcspn(message, "\n")] = '\0';

    strcpy(data, message);

    int data_len = strlen(data);

    ipv4_head_push(iph, "127.0.0.1", "127.0.0.1", IPPROTO_UDP, data_len);

    packet_size = sizeof(struct iphdr) + udp_head_push(udph, SERVER_PORT, CLIENT_PORT, data_len);;

    if (sendto(fd, packet, packet_size, 0, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("sendto message error");
        exit(EXIT_FAILURE);
    }

    if (strcmp(message, "exit") == 0) {
        break;
    }

    while (1) {
        char recv_buf[512];
        memset(recv_buf, 0, sizeof(recv_buf));

        ssize_t bytes_read = recv(fd, recv_buf, sizeof(recv_buf), 0);


        if(parse_packet(recv_buf, bytes_read, CLIENT_PORT) == -1) continue;
      
        struct iphdr *ip_reply = (struct iphdr*)recv_buf;
        int ip_header_len = ip_reply->ihl * 4;
        struct udphdr *udp_reply = (struct udphdr*)(recv_buf + ip_header_len);

        if (ntohs(udp_reply->source) == SERVER_PORT &&
            ntohs(udp_reply->dest) == CLIENT_PORT) {

            int reply_len = ntohs(udp_reply->len) - sizeof(struct udphdr);
            char *reply_data = recv_buf + ip_header_len + sizeof(struct udphdr);

            printf("Ответ от сервера: %.*s\n", reply_len, reply_data);
            break;
        }
    }
}

    close(fd);


    return 0;
}