CC = gcc
CFLAGS = -Wall -Wextra -g -O2

TARGETS = server client_tcp client_udp

all: $(TARGETS)

server: server.o
	$(CC) $(CFLAGS) -o server server.o

client_tcp: client_tcp.o
	$(CC) $(CFLAGS) -o client_tcp client_tcp.o

client_udp: client_udp.o
	$(CC) $(CFLAGS) -o client_udp client_udp.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGETS)

.PHONY: all clean