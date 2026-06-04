CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
TARGET = main
SRC = main.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean fclean re

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all
