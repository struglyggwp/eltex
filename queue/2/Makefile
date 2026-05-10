CC := gcc

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin

CFLAGS := -Wall -Wextra -Werror -std=c11 -I$(INC_DIR)
LDFLAGS := -pthread
LDLIBS := -lncursesw

CLIENT_TARGET := $(BIN_DIR)/chat
SERVER_TARGET := $(BIN_DIR)/server

CLIENT_SRCS := \
	$(SRC_DIR)/run.c \
	$(SRC_DIR)/client.c \
	$(SRC_DIR)/ui.c \
	$(SRC_DIR)/events.c \
	$(SRC_DIR)/sender.c \
	$(SRC_DIR)/receiver.c

SERVER_SRCS := $(SRC_DIR)/server.c

CLIENT_OBJS := $(CLIENT_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
SERVER_OBJS := $(SERVER_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all run run-server clean rebuild

all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(SERVER_TARGET): $(SERVER_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

run: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all

$(BUILD_DIR)/run.o $(BUILD_DIR)/sender.o: $(INC_DIR)/client.h $(INC_DIR)/common.h
$(BUILD_DIR)/client.o: $(INC_DIR)/client.h $(INC_DIR)/events.h $(INC_DIR)/ui.h $(INC_DIR)/common.h
$(BUILD_DIR)/events.o $(BUILD_DIR)/receiver.o: $(INC_DIR)/events.h $(INC_DIR)/client.h $(INC_DIR)/common.h
$(BUILD_DIR)/server.o: $(INC_DIR)/server.h $(INC_DIR)/common.h
$(BUILD_DIR)/ui.o: $(INC_DIR)/ui.h $(INC_DIR)/common.h
