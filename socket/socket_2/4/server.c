#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <time.h>

#define PORT 7777
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main(void) {
    int listen_tcp, sock_udp, epoll_fd;
    struct sockaddr_in addr;
    struct epoll_event ev, events[MAX_EVENTS];

    listen_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_tcp == -1) {
        perror("socket tcp error");
        exit(EXIT_FAILURE);
    }

    sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_udp == -1) {
        perror("socket udp error");
        close(listen_tcp);
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(listen_tcp, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(sock_udp, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listen_tcp, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind tcp error");
        close(listen_tcp);
        close(sock_udp);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_tcp, 5) == -1) {
        perror("listen error");
        close(listen_tcp);
        close(sock_udp);
        exit(EXIT_FAILURE);
    }

    if (bind(sock_udp, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind udp error");
        close(listen_tcp);
        close(sock_udp);
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        close(listen_tcp);
        close(sock_udp);
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_tcp;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_tcp, &ev) == -1) {
        perror("epoll_ctl tcp error");
        close(listen_tcp);
        close(sock_udp);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = sock_udp;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_udp, &ev) == -1) {
        perror("epoll_ctl udp error");
        close(listen_tcp);
        close(sock_udp);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен. Ожидание событий...\n");

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        if (nfds == -1) {
            perror("epoll_wait error");
            break;
        }

        for (int i = 0; i < nfds; i++) {

            time_t raw_time = time(NULL);
            char *time_str = ctime(&raw_time);

            if (events[i].data.fd == listen_tcp) {
                int client_sock = accept(listen_tcp, NULL, NULL);

                if (client_sock == -1) {
                    perror("accept error");
                    continue;
                }

                if (send(client_sock, time_str, strlen(time_str), 0) == -1) {
                    perror("send tcp error");
                    close(client_sock);
                    continue;
                }

                printf("TCP клиент подключен. Время отправлено: %s", time_str);

                close(client_sock);
            }

            else if (events[i].data.fd == sock_udp) {
                char buf[BUFFER_SIZE];
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);

                ssize_t n = recvfrom(sock_udp, buf, sizeof(buf) - 1, 0,(struct sockaddr *) &client_addr, &client_len);

                if (n == -1) {
                    perror("recvfrom udp error");
                    continue;
                }

                buf[n] = '\0';

                printf("UDP запрос получен: %s\n", buf);

                if (sendto(sock_udp, time_str, strlen(time_str), 0,
                           (struct sockaddr *)&client_addr,
                           client_len) == -1) {
                    perror("sendto udp error");
                    continue;
                }

                printf("UDP время отправлено: %s", time_str);
            }
        }
    }

    close(listen_tcp);
    close(sock_udp);
    close(epoll_fd);

    return 0;
}