
CC := clang
CFLAGS := -I.. -O3 -Wall -Wextra
PYTHON := python3

SRC_DIR := .
BUILD_DIR := build
RESULTS_DIR := results
IMAGES_DIR := images

COMMON_SRCS := ../tensors.c
COMMON_OBJS := $(COMMON_SRCS:%.c=$(BUILD_DIR)/%.o)

# Specify solver name as input
SOLVER ?=

SOLVER_SRC := $(SRC_DIR)/$(SOLVER).c
SOLVER_BIN := $(BUILD_DIR)/$(SOLVER)
SOLVER_OUT := $(RESULTS_DIR)/$(SOLVER).bin
SOLVER_LOG := $(RESULTS_DIR)/$(SOLVER).log
SOLVER_GIF := $(IMAGES)/$(SOLVER).gif

# Common Library
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(BUILD_DIR)
	$(CC) $(CLAGS) -c $< -o $@

# Compile
$(BUILD_DIR)/%: $(SRC_DIR)/%.c $(COMMON_OBJS) | $(BUILD_DIR)	
	$(CC) $(CFLAGS) $(COMMON_OBJS) $< -o $@

# Run
$(RESULTS_DIR)/%.bin: $(BUILD_DIR)/% | $(RESULTS_DIR)
	./$< > $(RESULTS_DIR)/$*.log
	mv $*.bin $(RESULTS_DIR)/

$(IMAGES_DIR)/%.gif: $(RESULTS_DIR)/%.bin plot_results.py | $(IMAGES_DIR)
	$(PYTHON) plot_results.py $<

%:
	@$(MAKE) --no-print-directory $(BUILD_DIR)/$@ SOLVER=$@

run-%:
	@$(MAKE) --no-print-directory $(RESULTS_DIR)/$*.bin SOLVER=$*

gif-%:
	@$(MAKE) --no-print-directory $(IMAGES_DIR)/$*.gif SOLVER=$*

$(BUILD_DIR) $(RESULTS_DIR) $(IMAGES_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(RESULTS_DIR) $(IMAGES_DIR)

