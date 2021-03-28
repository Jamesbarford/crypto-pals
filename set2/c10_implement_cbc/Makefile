SRC = ./
OBJ_DIR = ./build
INCLUDES = ../../includes
TARGET = cbc.out
CFLAGS = -Wall -Wextra -Wpedantic -Wno-shadow -O0 -g
CC = cc

$(OBJ_DIR)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(INCLUDES)/readinfile/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(INCLUDES)/base64tohex/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(TARGET)

clean:
	rm $(OBJ_DIR)/*.o
	rm $(TARGET)
	
OBJLIST = $(OBJ_DIR)/main.o \
          $(OBJ_DIR)/cbc.o \
          $(OBJ_DIR)/readinfile.o \
          $(OBJ_DIR)/base64tohex.o

$(TARGET): $(OBJLIST)
	$(CC) -o $(TARGET) $(OBJLIST)

$(SRC)/main.o: $(SRC)/main.c $(SRC)/cbc.h $(INCLUDES)/readinfile.h
$(SRC)/cbc.o: $(SRC)/cbc.h
$(INCLUDES)/readinfile.o: $(INCLUDES)/readinfile.c $(INCLUDES)/readinfile.h 
$(INCLUDES)/base64tohex.o: $(INCLUDES)/base64tohex.c $(INCLUDES)/base64tohex.h 
