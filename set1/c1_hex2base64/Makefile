OBJ_DIR = ./build
SRC = ./
INCLUDES = ../../includes
TARGET = hex2base64.out
CC_FLAGS = -Wall -Werror -Wextra -Wpedantic -Wno-shadow -g -O0
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
          $(OBJ_DIR)/hex2base64.o \
          $(OBJ_DIR)/readinfile.o

$(TARGET): $(OBJLIST)
	$(CC) -o $(TARGET) $(OBJLIST)

$(OBJ_DIR)/main.o: $(SRC)/hex2base64.h $(INCLUDES)/readinfile/readinfile.h
$(OBJ_DIR)/hex2base64.o: $(SRC)/hex2base64.c $(SRC)/hex2base64.h
$(OBJ_DIR)/readinfile.o: $(INCLUDES)/readinfile/readinfile.c $(INCLUDES)/readinfile/readinfile.h
