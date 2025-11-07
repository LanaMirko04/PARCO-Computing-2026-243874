CC       := gcc
CFLAGS   := -std=c11 -Wall -Wextra -Werror -O2 -Iinclude
# Add dependencies
CFLAGS += -Ilib/arena/include
LDFLAGS  := -Llib/arena/build -larena

SRC_DIR   := src
BUILD_DIR := build

SOURCES   := $(wildcard $(SRC_DIR)/*.c)
OBJECTS   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

EXEC_NAME := pgm
TARGET   := $(BUILD_DIR)/$(EXEC_NAME)

GREEN  := \033[1;32m
RED    := \033[1;31m
YELLOW := \033[1;33m
BLUE   := \033[1;34m
RESET  := \033[0m

.PHONY: all deps clean help

all: $(TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(TARGET): $(OBJECTS)
	@echo  "$(BLUE)>>>$(RESET) Linking executable: $(EXEC_NAME)"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) && echo "$(GREEN)Build successful!$(RESET)" || (echo "$(RED)Build failed!$(RESET)" && exit 1)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo  "$(BLUE)>>>$(RESET) Compiling $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

deps:
	@echo  "$(BLUE)>>>$(RESET) Building dependencies..."
	@echo "$(BLUE)>>>$(RESET) Building lib/arena..."
	@$(MAKE) -C lib/arena
	@echo "$(GREEN)Dependencies built successfully!$(RESET)"


clean:
	@echo  "$(BLUE)>>>$(RESET) Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)

help:
	@echo ""
	@echo "$(YELLOW)Available targets:$(RESET)"
	@echo "  $(GREEN)make$(RESET)          	- Build the project"
	@echo "  $(GREEN)make deps$(RESET)		- Build the dependencies"
	@echo "  $(GREEN)make clean$(RESET)     - Remove build artifacts"
	@echo "  $(GREEN)make help$(RESET)      - Show this help message"
	@echo ""
