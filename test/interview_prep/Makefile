CC := gcc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Werror -pedantic
SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS := -lm

SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build

COMMON_SRC := $(SRC_DIR)/ring_buffer.c $(SRC_DIR)/dsp_control.c $(SRC_DIR)/sed_extractor.c
COMMON_OBJ := $(COMMON_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean demo test sanitize

all: demo test

demo: $(BUILD_DIR)/demo
	$(BUILD_DIR)/demo

test: $(BUILD_DIR)/test_ring_buffer $(BUILD_DIR)/test_sed_extractor
	$(BUILD_DIR)/test_ring_buffer
	$(BUILD_DIR)/test_sed_extractor

sanitize: CFLAGS += $(SAN_FLAGS)
sanitize: LDFLAGS += $(SAN_FLAGS)
sanitize: clean all

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/demo: $(BUILD_DIR)/demo_main.o $(COMMON_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/demo_main.o: $(SRC_DIR)/demo_main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/test_ring_buffer: $(TEST_DIR)/test_ring_buffer.c $(BUILD_DIR)/ring_buffer.o
	$(CC) $(CFLAGS) -I$(SRC_DIR) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/test_sed_extractor: $(TEST_DIR)/test_sed_extractor.c $(BUILD_DIR)/sed_extractor.o
	$(CC) $(CFLAGS) -I$(SRC_DIR) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)
