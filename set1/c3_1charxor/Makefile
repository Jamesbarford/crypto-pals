LINK_TARGET = 1charxor.out
CC_FLAGS = -Wall -Werror -Wextra -Wpedantic -Wno-shadow -g -O0
CC = cc

$(LINK_TARGET): 1charxor.c
	$(CC) $(CC_FLAGS) -o $@ $^

clean:
	rm -rf $(LINK_TARGET)
	@echo "clean done ✨"
