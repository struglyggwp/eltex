CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGETS = receiver sender
OBJS = receiver.o sender.o

all: $(TARGETS)

receiver: receiver.o
	$(CC) $(CFLAGS) -o receiver receiver.o

sender: sender.o
	$(CC) $(CFLAGS) -o sender sender.o

receiver.o: receiver.c
	$(CC) $(CFLAGS) -c receiver.c

sender.o: sender.c
	$(CC) $(CFLAGS) -c sender.c

clean:
	rm -f $(TARGETS) $(OBJS)

fclean: clean
	rm -f *~

re: fclean all