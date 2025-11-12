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
	@printf "$(BLUE)>>>$(RESET) Linking executable: $(EXEC_NAME)\n"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) && printf "$(GREEN)Build successful!$(RESET)\n" || (printf "$(RED)Build failed!$(RESET)\n" && exit 1)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@printf "$(BLUE)>>>$(RESET) Compiling $<\n"
	@$(CC) $(CFLAGS) -c -o $@ $<

deps:
	@printf "$(BLUE)>>>$(RESET) Building dependencies...\n"
	@printf "$(BLUE)>>>$(RESET) Building lib/arena...\n"
	@$(MAKE) -C lib/arena
	@printf "$(GREEN)Dependencies built successfully!$(RESET)\n"


clean:
	@printf  "$(BLUE)>>>$(RESET) Cleaning build artifacts...\n"
	@rm -rf $(BUILD_DIR)

help:
	@printf "\n"
	@printf "$(YELLOW)Available targets:$(RESET)\n"
	@printf "  $(GREEN)make$(RESET)          	- Build the project\n"
	@printf "  $(GREEN)make deps$(RESET)		- Build the dependencies\n"
	@printf "  $(GREEN)make clean$(RESET)     - Remove build artifacts\n"
	@printf "  $(GREEN)make help$(RESET)      - Show this help message\n"
	@printf "\n"
