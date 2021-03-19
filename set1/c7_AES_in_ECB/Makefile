OBJ_DIR	= ./build
SRC = ./
TARGET = decrypt.out
INCLUDES = ../../includes
CFLAGS = -Wall -Wextra -Wpedantic -Wno-shadow -O0 -g
CC = cc

${OBJ_DIR}/%.o: ${SRC}/%.c
	${CC} -c ${CFLAGS} -o $@ $<

${OBJ_DIR}/%.o: ${INCLUDES}/base64tohex/%.c
	${CC} -c ${CFLAGS} -o $@ $<

all: ${TARGET}

clean:
	rm ${OBJ_DIR}/*.o
	rm ${TARGET}

OBJLIST = ${OBJ_DIR}/decrypt.o \
          ${OBJ_DIR}/main.o \
          ${OBJ_DIR}/encrypt_errors.o \
          ${OBJ_DIR}/base64tohex.o

${TARGET}: ${OBJLIST}
	${CC} -o ${TARGET} ${OBJLIST} -lssl -lcrypto

${OBJ_DIR}/decrypt.o: ${SRC}/decrypt.c ${SRC}/decrypt.h ${SRC}encrypt_errors.h
${OBJ_DIR}/main.o: ${SRC}/main.c ${SRC}/decrypt.h
${OBJ_DIR}/encrypt_errors.o: ${SRC}/encrypt_errors.c ${SRC}/encrypt_errors.h
${OBJ_DIR}/base64tohex.o: ${INCLUDES}/base64tohex/base64tohex.c ${INCLUDES}/base64tohex/base64tohex.h
