OBJ_DIR	= ./build
SRC = ./
INCLUDES = ../../includes
TARGET = decrypt.out
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

OBJLIST = ${OBJ_DIR}/main.o \
          ${OBJ_DIR}/base64tohex.o

${TARGET}: ${OBJLIST}
	${CC} -o ${TARGET} ${OBJLIST}

${OBJ_DIR}/main.o: ${SRC}/main.c ${INCLUDES}/base64tohex/base64tohex.h
${OBJ_DIR}/base64tohex.o: ${INCLUDES}/base64tohex/base64tohex.c ${INCLUDES}/base64tohex/base64tohex.h
