#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h> 

#define SERVER_PORT 7777
#define CLIENT_PORT 8888
#define BUFFER_SIZE 512

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

    char packet[512];


    memset(packet, 0, 512);

    struct iphdr *iph = (struct iphdr*) packet;
    struct udphdr *udph = (struct udphdr*)(packet + sizeof(struct iphdr));
    char *data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);

    iph->ttl = 64;
    iph->frag_off = 0;
    iph->id = 0;
    iph->version = 4;
    iph->tos = 0;
    iph->protocol = IPPROTO_UDP;
    iph->ihl = 5;
    iph->saddr = inet_addr("127.0.0.1");
    iph->daddr = inet_addr("127.0.0.1");
    iph->check = 0;

    udph->source = htons(CLIENT_PORT);
    udph->dest = htons(SERVER_PORT);
    udph->check = 0;

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 0;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int packet_size = ntohs(iph->tot_len);

    while(1){
    printf("Напишите echo-сообщение\n");
    
    memset(message, 0, sizeof(message));

    if (fgets(message, sizeof(message), stdin) == NULL) {
        perror("fgets error");
        continue;
    }

    udph->len = htons(sizeof(struct udphdr) + strlen(data));
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data));

    message[strcspn(message, "\n")] = '\0';

    strcpy(data, message);

    int data_len = strlen(data);

    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + data_len);
    udph->len = htons(sizeof(struct udphdr) + data_len);
    packet_size = sizeof(struct iphdr) + sizeof(struct udphdr) + data_len;

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
        if (bytes_read < 0) {
            continue;
        }

        struct iphdr *ip_reply = (struct iphdr*)recv_buf;

        if (ip_reply->protocol != IPPROTO_UDP) {
            continue;
        }

        int ip_header_len = ip_reply->ihl * 4;

        if (bytes_read < ip_header_len + (int)sizeof(struct udphdr)) {
            continue;
        }

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