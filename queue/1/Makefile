CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L
LDLIBS = -lrt

OBJ_DIR = obj
BIN_DIR = bin

TARGETS = $(BIN_DIR)/posix_server \
          $(BIN_DIR)/posix_client \
          $(BIN_DIR)/systemv_server \
          $(BIN_DIR)/systemv_client

all: $(TARGETS)

$(BIN_DIR)/posix_server: $(OBJ_DIR)/posix_server.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

$(BIN_DIR)/posix_client: $(OBJ_DIR)/posix_client.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

$(BIN_DIR)/systemv_server: $(OBJ_DIR)/systemv_server.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/systemv_client: $(OBJ_DIR)/systemv_client.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run-posix-server: $(BIN_DIR)/posix_server
	./$(BIN_DIR)/posix_server

run-posix-client: $(BIN_DIR)/posix_client
	./$(BIN_DIR)/posix_client

run-systemv-server: $(BIN_DIR)/systemv_server
	./$(BIN_DIR)/systemv_server

run-systemv-client: $(BIN_DIR)/systemv_client
	./$(BIN_DIR)/systemv_client

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

.PHONY: all clean rebuild run-posix-server run-posix-client run-systemv-server run-systemv-client