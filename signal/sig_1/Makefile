CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

all: receiver sender

receiver: receiver.c
	$(CC) $(CFLAGS) receiver.c -o receiver

sender: sender.c
	$(CC) $(CFLAGS) sender.c -o sender

clean:
	rm -f receiver sender