CC := gcc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Werror -pedantic
LDFLAGS := -lm

SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build

SRC := $(SRC_DIR)/fft.c $(SRC_DIR)/mel.c $(SRC_DIR)/sed_open.c $(SRC_DIR)/wav_io.c
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean test

all: $(BUILD_DIR)/sed_clone_open $(BUILD_DIR)/test_sed_open

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/sed_clone_open: $(BUILD_DIR)/main.o $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/test_sed_open: $(TEST_DIR)/test_sed_open.c $(BUILD_DIR)/fft.o $(BUILD_DIR)/mel.o $(BUILD_DIR)/sed_open.o
	$(CC) $(CFLAGS) -I$(SRC_DIR) $^ -o $@ $(LDFLAGS)

test: $(BUILD_DIR)/test_sed_open
	$(BUILD_DIR)/test_sed_open

clean:
	rm -rf $(BUILD_DIR)
