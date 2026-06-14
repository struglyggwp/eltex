CC := gcc

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin

CFLAGS := -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L -I$(INC_DIR)
LDFLAGS := -pthread
CLIENT_LDLIBS := -lncursesw -lrt
SERVER_LDLIBS := -lrt

CLIENT_TARGET := $(BIN_DIR)/chat
SERVER_TARGET := $(BIN_DIR)/server

CLIENT_SRCS := \
	$(SRC_DIR)/run.c \
	$(SRC_DIR)/client.c \
	$(SRC_DIR)/ui.c \
	$(SRC_DIR)/events.c \
	$(SRC_DIR)/sender.c \
	$(SRC_DIR)/receiver.c \
	$(SRC_DIR)/shared.c

SERVER_SRCS := \
	$(SRC_DIR)/server.c \
	$(SRC_DIR)/shared.c

CLIENT_OBJS := $(CLIENT_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/client_%.o)
SERVER_OBJS := $(SERVER_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/server_%.o)

.PHONY: all run-server run-client clean rebuild

all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(CLIENT_LDLIBS) -o $@

$(SERVER_TARGET): $(SERVER_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(SERVER_LDLIBS) -o $@

$(BUILD_DIR)/client_%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(BUILD_DIR)/server_%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

run-client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET) user

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all
