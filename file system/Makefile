CC := gcc
CFLAGS := -Wall -Wextra -Werror -Wno-error=format-truncation -std=c11 -Iinclude -MMD -MP
PICFLAGS := -fPIC
LDFLAGS := -Wl,-rpath,'$$ORIGIN'
LDLIBS := -lncurses

BUILD_DIR := build
LIB_NAME := libmc.so
APP_NAME := main

LIB_SRCS := \
	src/mc_destroy_ui.c \
	src/mc_draw_panel.c \
	src/mc_free_files.c \
	src/mc_handle_input.c \
	src/mc_init_ui.c \
	src/mc_list_dir.c \
	src/mc_render_ui.c \
	src/mc_run.c \
	src/mc_xstrdup.c

LIB_OBJS := $(LIB_SRCS:src/%.c=$(BUILD_DIR)/%.o)
LIB_DEPS := $(LIB_OBJS:.o=.d)
APP_OBJ := $(BUILD_DIR)/main.o
APP_DEP := $(APP_OBJ:.o=.d)

.PHONY: all clean

all: $(APP_NAME)

$(APP_NAME): $(APP_OBJ) $(LIB_NAME)
	$(CC) $(APP_OBJ) -L. -lmc $(LDFLAGS) $(LDLIBS) -o $@

$(LIB_NAME): $(LIB_OBJS)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PICFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME) libmc.a $(APP_NAME)

-include $(LIB_DEPS) $(APP_DEP)
