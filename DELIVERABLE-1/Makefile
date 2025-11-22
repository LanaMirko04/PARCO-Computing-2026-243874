CC       := gcc
CFLAGS   := -std=c11 -Wall -Wextra -Werror -O3 -Iinclude -D_POSIX_C_SOURCE=200809L
# CFLAGS   := -std=c11 -g -Wall -Wextra -Werror -O0 -Iinclude

# Add dependencies
CFLAGS += -Ilib/arena/include -Ilib/slog/include
LDFLAGS  := -lm -Llib/arena/build -Llib/slog/build -larena -lslog

ifeq ($(shell uname), Darwin)
	CFLAGS += -Xpreprocessor -I/opt/homebrew/opt/libomp/include
	LDFLAGS += -L/opt/homebrew/opt/libomp/lib -lomp
else
	CFLAGS += -fopenmp
endif

SRC_DIR   := src
BUILD_DIR := build

SOURCES   := $(wildcard $(SRC_DIR)/*.c)
OBJECTS   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

EXEC_NAME := spvm
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
	@printf "$(BLUE)>>>$(RESET) Building lib/slog...\n"
	@$(MAKE) -C lib/slog
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
