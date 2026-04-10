CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11
TARGET := main
SRC := main.c

.PHONY: all clean re

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

re: clean all
