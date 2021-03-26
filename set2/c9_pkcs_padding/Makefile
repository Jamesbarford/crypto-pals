TARGET = pkspadding.out
CC_FLAGS = -Wall -Werror -Wextra -Wpedantic -Wno-shadow -g -O0
CC = cc

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CC_FLAGS) -o $@ $^

clean:
	rm -rf $(TARGET)
	@echo "clean done ✨"

