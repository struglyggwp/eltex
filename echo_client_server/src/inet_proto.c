#include "inet_proto.h"

void ipv4_head_push(struct iphdr *iph, char *ip_dest, char *ip_src, int ip_proto, ssize_t payload_len){

    iph->ttl = 64;
    iph->frag_off = 0;
    iph->id = 0;
    iph->version = 4;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + payload_len);
    iph->tos = 0;
    iph->protocol = ip_proto;
    iph->ihl = 5;
    iph->saddr = inet_addr(ip_src);
    iph->daddr = inet_addr(ip_dest);
    iph->check = 0;
}

ssize_t udp_head_push(struct udphdr *udph, uint16_t port_dest, uint16_t port_src, ssize_t payload_len){

    udph->source = htons(port_src);
    udph->dest = htons(port_dest);
    udph->check = 0;
    udph->len = htons(sizeof(struct udphdr) + payload_len);

    return (sizeof(struct udphdr) + payload_len);
}


int parse_packet(char *packet, ssize_t bytes, uint16_t port_dest)
{
    if (packet == NULL) return -1;

    if (bytes <= 0) return -1;

    if (bytes < (ssize_t)sizeof(struct iphdr)) return -1;

    struct iphdr *iph = (struct iphdr *)packet;

    if (iph->version != 4) return -1;

    if (iph->ihl < 5) return -1;

    if (iph->protocol != IPPROTO_UDP) return -1;

    int ip_header_len = iph->ihl * 4;

    if (bytes < ip_header_len + (int)sizeof(struct udphdr)) return -1;

    int ip_total_len = ntohs(iph->tot_len);

    if (ip_total_len < ip_header_len + (int)sizeof(struct udphdr)) return -1;

    if (ip_total_len > bytes) return -1;

    struct udphdr *udph = (struct udphdr *)(packet + ip_header_len);

    if (ntohs(udph->dest) != port_dest) return -1;

    int udp_len = ntohs(udph->len);

    if (udp_len < (int)sizeof(struct udphdr)) return -1;

    if (udp_len > ip_total_len - ip_header_len) return -1;

    int payload_len = udp_len - sizeof(struct udphdr);

    if (payload_len <= 0) return -1;

    return 0;
}