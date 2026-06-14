
CC = gcc

CFLAGS = -Wall -I.

AR = ar

LDFLAGS = -L. -lcalc

EXEC_NAME = calculator

LIB_NAME = libcalc.a


LIB_SRCS = plus.c mines.c multi.c divide.c

LIB_OBJS = $(LIB_SRCS:.c=.o)


MAIN_SRC = main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)


all: $(EXEC_NAME)

$(EXEC_NAME): $(MAIN_OBJ) $(LIB_NAME)

	$(CC) $(CFLAGS) -o $(EXEC_NAME) $(MAIN_OBJ) $(LDFLAGS)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) rcs $(LIB_NAME) $(LIB_OBJS)

$(MAIN_OBJ): $(MAIN_SRC)
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o $(MAIN_OBJ)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean
clean:
	rm -f $(EXEC_NAME) $(LIB_NAME) $(MAIN_OBJ) $(LIB_OBJS)

