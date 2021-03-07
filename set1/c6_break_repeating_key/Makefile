OUT_DIR	= output
SRC = ./
LINK_TARGET = break_repeating_key.out
SRC_FILES = main.c ./base64tohex/base64tohex.c
OBJS = $(patsubst $(SRC)/%.c, $(OUT_DIR)/%.o, $(SRC_FILES))
CC_FLAGS = -std=gnu17 -Wall -Werror -Wextra -Wpedantic -Wno-shadow -g -O0
CC = gcc

all: $(LINK_TARGET)
	@echo "compilation success ✅"

$(LINK_TARGET): $(OBJS)
	$(CC) $(CC_FLAGS) -o $@ $^

$(OUT_DIR)/%.o: ./%.c
	$(CC) $(CC_FLAGS) -o $@ -c $<

$(OUT_DIR)/%.o: $(SRC)/%.c
	$(CC) $(CC_FLAGS) -o $@ -c $<

clean:
	rm -rf $(LINK_TARGET)
	@echo "clean done ✨"
