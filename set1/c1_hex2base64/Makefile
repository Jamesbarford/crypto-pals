LINK_TARGET = hex2base64.out
CC_FLAGS = -std=gnu17 -Wall -Werror -Wextra -Wpedantic -Wno-shadow -g -O0
CC = gcc

all: $(LINK_TARGET)
	@echo "compilation success ✅"

$(LINK_TARGET): hex2base64.c
	$(CC) $(CC_FLAGS) -o $@ $^

clean:
	rm -rf $(LINK_TARGET)
	@echo "clean done ✨"