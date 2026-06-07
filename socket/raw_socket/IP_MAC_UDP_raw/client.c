#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h> 
#include <linux/if_packet.h> // Для struct sockaddr_ll
#include <net/ethernet.h>    // Для struct ethhdr и константы ETH_P_IP
#include <net/if.h>          // Для функции if_nametoindex

#define SERVER_PORT 7777
#define CLIENT_PORT 8888
#define BUFFER_SIZE 512

unsigned short checksum(unsigned short *ptr, int nbytes) {
    long sum = 0;
    unsigned short oddbyte;
    unsigned short answer;

    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((unsigned char *)&oddbyte) = *(unsigned char *)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (unsigned short)~sum;
    return answer;
}



int main() {
   
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    char packet[512];
    memset(packet, 0, 512);

    unsigned char src_mac[6] = {0x08, 0x00, 0x27, 0xC9, 0x1A, 0xAE};
    unsigned char dest_mac[6] = {0x08, 0x00, 0x27, 0x63, 0xB0, 0x05};

    struct ethhdr *eth = ( struct ethhdr*) packet;
    struct iphdr *iph = (struct iphdr*)(packet + sizeof(struct ethhdr));
    struct udphdr *udph = (struct udphdr*)(packet + sizeof( struct ethhdr) + sizeof(struct iphdr) );

    char *data = packet + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data, "Hello server");

    memcpy(eth->h_source, src_mac, 6);
    memcpy(eth->h_dest, dest_mac, 6);

    eth->h_proto = htons(ETH_P_IP);

 // заполнение ip заголовка

    iph->ttl = 64;
    iph->frag_off = 0;
    iph->id = 0;
    iph->version = 4;
    iph->tos = 0;
    iph->protocol = IPPROTO_UDP;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data));
    iph->ihl = 5;
    iph->saddr = inet_addr("192.168.1.50");
    iph->daddr = inet_addr("192.168.1.102");
    iph->check = checksum((unsigned short *)iph, sizeof(struct iphdr));

 // заполнение udp заголовка
    udph->source = htons(CLIENT_PORT);
    udph->dest = htons(SERVER_PORT);
    udph->len = htons(sizeof(struct udphdr) + strlen(data));
    udph->check = 0;

    struct sockaddr_ll server_addr;
    server_addr.sll_family = AF_PACKET;
    server_addr.sll_ifindex = if_nametoindex("eth0");
    server_addr.sll_halen = ETH_ALEN;
    memcpy(server_addr.sll_addr, dest_mac, 6);  

    int packet_size = sizeof(struct ethhdr) + ntohs(iph->tot_len);;

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

        struct ethhdr *eth_reply = (struct ethhdr*)recv_buf;

        if(ntohs(eth_reply->h_proto) == ETH_P_IP){

            struct iphdr *ip_reply = (struct iphdr*)(recv_buf + sizeof(struct ethhdr));

        
            if(ip_reply->protocol == IPPROTO_UDP){
                struct udphdr *udp_reply = (struct udphdr*)(recv_buf + sizeof(struct ethhdr) + (ip_reply->ihl * 4));

                if(ntohs(udp_reply->dest) == CLIENT_PORT){
                    char *reply_data = (char*)(recv_buf + sizeof(struct ethhdr) + (ip_reply->ihl * 4) + sizeof(struct udphdr));
                    printf("Ответ от сервера:%s\n", reply_data);
                    break;
            }
        }
    }

    }

    close(fd);


    return 0;
}