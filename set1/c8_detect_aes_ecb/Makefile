OBJ_DIR	= ./build
SRC = ./
TARGET = decrypt.out
CFLAGS = -Wall -Wextra -Wpedantic -Wno-shadow -O0 -g
CC = cc

${OBJ_DIR}/%.o: ${SRC}/%.c
	${CC} -c ${CFLAGS} -o $@ $<

all: ${TARGET}

clean:
	rm ${OBJ_DIR}/*.o
	rm ${TARGET}

OBJLIST = ${OBJ_DIR}/main.o

${TARGET}: ${OBJLIST}
	${CC} -o ${TARGET} ${OBJLIST}

${OBJ_DIR}/main.o: ${SRC}/main.c
