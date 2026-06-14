CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L
CPPFLAGS = -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

POSIX_SERVER = $(BIN_DIR)/posix_server
POSIX_CLIENT = $(BIN_DIR)/posix_client
SYSV_SERVER = $(BIN_DIR)/systemv_server
SYSV_CLIENT = $(BIN_DIR)/systemv_client

TARGETS = $(POSIX_SERVER) $(POSIX_CLIENT) $(SYSV_SERVER) $(SYSV_CLIENT)

all: $(TARGETS)

$(POSIX_SERVER): $(OBJ_DIR)/posix_server.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(POSIX_CLIENT): $(OBJ_DIR)/posix_client.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(SYSV_SERVER): $(OBJ_DIR)/systemv_server.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(SYSV_CLIENT): $(OBJ_DIR)/systemv_client.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c include/common.h | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run-posix-server: $(POSIX_SERVER)
	./$(POSIX_SERVER)

run-posix-client: $(POSIX_CLIENT)
	./$(POSIX_CLIENT)

run-sysv-server: $(SYSV_SERVER)
	./$(SYSV_SERVER)

run-sysv-client: $(SYSV_CLIENT)
	./$(SYSV_CLIENT)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

.PHONY: all clean rebuild run-posix-server run-posix-client run-sysv-server run-sysv-client
