CC := gcc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Werror -pedantic
LDFLAGS := -lm
SRC := src
TESTS := tests
BUILD := build

COMMON_OBJ := $(BUILD)/ring_buffer_adv.o $(BUILD)/diagnostics.o

.PHONY: all clean test analyze run

all: $(BUILD)/pingpong_dma $(BUILD)/block_graph_runtime $(BUILD)/test_ring_buffer_adv $(BUILD)/test_diagnostics

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/ring_buffer_adv.o: $(SRC)/ring_buffer_adv.c $(SRC)/ring_buffer_adv.h | $(BUILD)
	$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

$(BUILD)/diagnostics.o: $(SRC)/diagnostics.c $(SRC)/diagnostics.h | $(BUILD)
	$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

$(BUILD)/pingpong_dma: $(SRC)/pingpong_dma.c $(BUILD)/diagnostics.o | $(BUILD)
	$(CC) $(CFLAGS) -I$(SRC) $^ -o $@ $(LDFLAGS)

$(BUILD)/block_graph_runtime: $(SRC)/block_graph_runtime.c | $(BUILD)
	$(CC) $(CFLAGS) -I$(SRC) $< -o $@ $(LDFLAGS)

$(BUILD)/test_ring_buffer_adv: $(TESTS)/test_ring_buffer_adv.c $(BUILD)/ring_buffer_adv.o | $(BUILD)
	$(CC) $(CFLAGS) -I$(SRC) $^ -o $@

$(BUILD)/test_diagnostics: $(TESTS)/test_diagnostics.c $(BUILD)/diagnostics.o | $(BUILD)
	$(CC) $(CFLAGS) -I$(SRC) $^ -o $@

run: all
	@echo "\n=== pingpong_dma ===" && ./$(BUILD)/pingpong_dma | sed -n '1,20p'
	@echo "\n=== block_graph_runtime ===" && ./$(BUILD)/block_graph_runtime | sed -n '1,20p'

test: all
	./$(BUILD)/test_ring_buffer_adv
	./$(BUILD)/test_diagnostics

analyze:
	@echo "\n[gcc -fanalyzer]"
	@if $(CC) -Q --help=optimizers 2>/dev/null | grep -q -- "-fanalyzer"; then \
		$(CC) -std=c11 -Wall -Wextra -pedantic -fanalyzer -I$(SRC) -c $(SRC)/ring_buffer_adv.c -o /tmp/ring_buffer_adv.o 2>/tmp/an1.log || true; \
		$(CC) -std=c11 -Wall -Wextra -pedantic -fanalyzer -I$(SRC) -c $(SRC)/diagnostics.c -o /tmp/diagnostics.o 2>/tmp/an2.log || true; \
		cat /tmp/an1.log /tmp/an2.log; \
	else \
		echo "gcc -fanalyzer not supported on this server"; \
	fi
	@echo "\n[cppcheck]"
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=warning,style,performance,portability --std=c11 --suppress=missingIncludeSystem $(SRC) $(TESTS); \
	else \
		echo "cppcheck not installed on this server"; \
	fi

clean:
	rm -rf $(BUILD)
