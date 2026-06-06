
CC = gcc


CFLAGS = -Wall -Wextra -g -O2


SERVER_BIN = server
CLIENT_BIN = client


SERVER_SRC = server.c
CLIENT_SRC = client.c

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

.PHONY: all clean