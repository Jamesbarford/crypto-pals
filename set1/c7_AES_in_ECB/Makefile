OBJ_DIR	= ./output
SRC = ./
TARGET = decrypt.out
CFLAGS = -Wall -Werror -Wextra -Wpedantic -Wno-shadow -O0 -g
CC = gcc-10
OPENSSL_DIR = /usr/local/opt/openssl@1.1
OPENSSL_SUPPORT = -I$(OPENSSL_DIR)/include -L$(OPENSSL_DIR)/lib

${OBJ_DIR}/%.o: ${SRC}/%.c
	${CC} -c ${CFLAGS} -o $@ $<

all: ${TARGET}

clean:
	rm ${OBJ_DIR}/*.o
	rm ${TARGET}

OBJLIST = ${OBJ_DIR}/decrypt.o \
          ${OBJ_DIR}/main.o \
          ${OBJ_DIR}/encrypt_errors.o

${TARGET}: ${OBJLIST}
	${CC} -o ${TARGET} ${OBJLIST} -lssl -lcrypto

${OBJ_DIR}/decrypt.o: ${SRC}/decrypt.c ${SRC}/decrypt.h ${SRC}encrypt_errors.h
${OBJ_DIR}/main.o: ${SRC}/main.c ${SRC}/decrypt.h
${OBJ_DIR}/encrypt_errors.o: ${SRC}/encrypt_errors.c ${SRC}/encrypt_errors.h
