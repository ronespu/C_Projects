CC := gcc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Werror -pedantic
BUILD := build
SRC := src

BINS := \
	$(BUILD)/01_memory_management \
	$(BUILD)/02_circular_buffer_audio \
	$(BUILD)/03_fixed_point_q15 \
	$(BUILD)/04_latency_budget \
	$(BUILD)/05_volatile_isr_sim \
	$(BUILD)/06_race_condition \
	$(BUILD)/07_debug_distortion_root_cause \
	$(BUILD)/08_command_control_dsp

.PHONY: all clean run

all: $(BINS)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/01_memory_management: $(SRC)/01_memory_management.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/02_circular_buffer_audio: $(SRC)/02_circular_buffer_audio.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@ -lm

$(BUILD)/03_fixed_point_q15: $(SRC)/03_fixed_point_q15.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/04_latency_budget: $(SRC)/04_latency_budget.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/05_volatile_isr_sim: $(SRC)/05_volatile_isr_sim.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/06_race_condition: $(SRC)/06_race_condition.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@ -lpthread

$(BUILD)/07_debug_distortion_root_cause: $(SRC)/07_debug_distortion_root_cause.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@ -lm

$(BUILD)/08_command_control_dsp: $(SRC)/08_command_control_dsp.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

run: all
	@for b in $(BINS); do \
		echo "\n=== $$b ==="; \
		$$b | sed -n '1,12p'; \
	done

clean:
	rm -rf $(BUILD)
