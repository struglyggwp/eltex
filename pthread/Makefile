CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -pthread -Iinclude
TARGET = app
BUILD_DIR = build
VPATH = . src
SRCS = main.c run.c running.c buy.c loading.c printshops.c
OBJS = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))

.PHONY: all clean run rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c include/pthread_shops.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all
