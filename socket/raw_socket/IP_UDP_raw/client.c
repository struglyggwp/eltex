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
   
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

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
    strcpy(data, "Hello server");

    iph->ttl = 64;
    iph->frag_off = 0;
    iph->id = 0;
    iph->version = 4;
    iph->tos = 0;
    iph->protocol = IPPROTO_UDP;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data));
    iph->ihl = 5;
    iph->saddr = inet_addr("127.0.0.1");
    iph->daddr = inet_addr("127.0.0.1");
    iph->check = 0;

    udph->source = htons(CLIENT_PORT);
    udph->dest = htons(SERVER_PORT);
    udph->len = htons(sizeof(struct udphdr) + strlen(data));
    udph->check = 0;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int packet_size = ntohs(iph->tot_len);

    if(sendto(fd, packet, packet_size, 0, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
        perror("sendto error");
        exit(EXIT_FAILURE);
    }

    printf("Пакет серверу отправлен\n");

    while(1){
        char recv_buf[512];
        memset(recv_buf, 0, 512);

        ssize_t bytes_read = recv(fd, recv_buf, 512, 0);
        if(bytes_read < 0) continue;

        struct iphdr *ip_reply = (struct iphdr*)recv_buf;

        if(ip_reply->protocol == IPPROTO_UDP){
            struct udphdr *udp_reply = (struct udphdr*)(recv_buf + (ip_reply->ihl * 4));

            if(ntohs(udp_reply->dest) == CLIENT_PORT){
                char *reply_data = (char*)(recv_buf + (ip_reply->ihl * 4) + sizeof(struct udphdr));
                printf("Ответ от сервера:%s\n", reply_data);
                break;
            }
        }

    }

    close(fd);


    return 0;
}