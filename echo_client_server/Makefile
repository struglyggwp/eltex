CC := gcc

CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -g
CPPFLAGS := -D_POSIX_C_SOURCE=200809L -Iinclude -MMD -MP

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

CLIENT_TARGET := $(BIN_DIR)/client
SERVER_TARGET := $(BIN_DIR)/server

ALL_SRCS := $(wildcard $(SRC_DIR)/*.c)

CLIENT_MAIN := $(SRC_DIR)/client.c
SERVER_MAIN := $(SRC_DIR)/server.c

SERVER_ONLY_SRCS := $(wildcard $(SRC_DIR)/server_*.c)
CLIENT_ONLY_SRCS := $(wildcard $(SRC_DIR)/client_*.c)

COMMON_SRCS := $(filter-out $(CLIENT_MAIN) $(SERVER_MAIN) $(SERVER_ONLY_SRCS) $(CLIENT_ONLY_SRCS),$(ALL_SRCS))

CLIENT_SRCS := $(CLIENT_MAIN) $(CLIENT_ONLY_SRCS) $(COMMON_SRCS)
SERVER_SRCS := $(SERVER_MAIN) $(SERVER_ONLY_SRCS) $(COMMON_SRCS)

CLIENT_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/client/%.o,$(CLIENT_SRCS))
SERVER_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/server/%.o,$(SERVER_SRCS))

DEPS := $(CLIENT_OBJS:.o=.d) $(SERVER_OBJS:.o=.d)

.PHONY: all clean rebuild run-server run-client dirs

all: dirs $(CLIENT_TARGET) $(SERVER_TARGET)

dirs:
	mkdir -p $(OBJ_DIR)/client $(OBJ_DIR)/server $(BIN_DIR)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/client/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/server/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run-server: $(SERVER_TARGET)
	sudo ./$(SERVER_TARGET)

run-client: $(CLIENT_TARGET)
	sudo ./$(CLIENT_TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

-include $(DEPS)