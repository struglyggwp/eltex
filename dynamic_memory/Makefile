CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = phonebook

SRCS = main.c book.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c book.h
	$(CC) $(CFLAGS) -c main.c -o main.o

book.o: book.c book.h
	$(CC) $(CFLAGS) -c book.c -o book.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

.PHONY: all run clean rebuild