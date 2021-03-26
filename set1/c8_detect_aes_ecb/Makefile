OBJ_DIR	= ./build
SRC = ./
INCLUDES = ../../includes
TARGET = decrypt.out
CFLAGS = -Wall -Wextra -Wpedantic -Wno-shadow -O0 -g
CC = cc

$(OBJ_DIR)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(INCLUDES)/readinfile/*.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(TARGET)

clean:
	rm $(OBJ_DIR)/*.o
	rm $(TARGET)

OBJLIST = $(OBJ_DIR)/main.o \
          $(OBJ_DIR)/readinfile.o

$(TARGET): $(OBJLIST)
	$(CC) -o $(TARGET) $(OBJLIST)

$(OBJ_DIR)/main.o: $(SRC)/main.c $(INCLUDES)/readinfile/readinfile.h
$(OBJ_DIR)/readinfile.o: $(INCLUDES)/readinfile/readinfile.c $(INCLUDES)/readinfile/readinfile.h
