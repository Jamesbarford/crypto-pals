/**
 * This CANNOT yet handle line breaks!!
 *
 *
 *
 * Simple base64 to hex converter
 *
 * Example text is from: https://en.wikipedia.org/wiki/Base64
 *
 * char *example = "TWFu";
 * int hex2 = 0;
 *
 * hex2 |= 19 << 18;
 * hex2 |= 22 << 12;
 * hex2 |= 5  << 6;
 * hex2 |= 46 << 0;
 *	
 * printf("0x%X\n", hex2); -> Man
 * 
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "base64tohex.h"

// hex -> 0x4D 0x61 0x6E
static inline int8_t base64_to_int(char c)
{
	switch(c)
	{
		case 'A': return 0;
		case 'B': return 1;
		case 'C': return 2;
		case 'D': return 3;
		case 'E': return 4;
		case 'F': return 5;
		case 'G': return 6;
		case 'H': return 7;
		case 'I': return 8;
		case 'J': return 9;
		case 'K': return 10;
		case 'L': return 11;
		case 'M': return 12;
		case 'N': return 13;
		case 'O': return 14;
		case 'P': return 15;
		case 'Q': return 16;
		case 'R': return 17;
		case 'S': return 18;
		case 'T': return 19;
		case 'U': return 20;
		case 'V': return 21;
		case 'W': return 22;
		case 'X': return 23;
		case 'Y': return 24;
		case 'Z': return 25;
		case 'a': return 26;
		case 'b': return 27;
		case 'c': return 28;
		case 'd': return 29;
		case 'e': return 30;
		case 'f': return 31;
		case 'g': return 32;
		case 'h': return 33;
		case 'i': return 34;
		case 'j': return 35;
		case 'k': return 36;
		case 'l': return 37;
		case 'm': return 38;
		case 'n': return 39;
		case 'o': return 40;
		case 'p': return 41;
		case 'q': return 42;
		case 'r': return 43;
		case 's': return 44;
		case 't': return 45;
		case 'u': return 46;
		case 'v': return 47;
		case 'w': return 48;
		case 'x': return 49;
		case 'y': return 50;
		case 'z': return 51;
		case '0': return 52;
		case '1': return 53;
		case '2': return 54;
		case '3': return 55;
		case '4': return 56;
		case '5': return 57;
		case '6': return 58;
		case '7': return 59;
		case '8': return 60;
		case '9': return 61;
		case '+': return 62;
		case '/': return 63;
		default:
			return 0;
	}
}

hexbuf_t *decodeB64(uint8_t *b64, size_t input_len)
{
	hexbuf_t *hex_block = NULL;
	uint64_t i = 0, j = 0;
	int32_t hex = 0;
	size_t output_len =  input_len / 4 * 3;

	if ((hex_block = malloc(output_len + sizeof(size_t))) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for decoded output: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	hex_block->size = output_len;

	while (i < input_len)
	{
		uint8_t c_1 = b64[i++];
		uint8_t c_2 = b64[i++];
		uint8_t c_3 = b64[i++];
		uint8_t c_4 = b64[i++];

		hex |= base64_to_int(c_1) << 18;
		hex |= base64_to_int(c_2) << 12;
		hex |= base64_to_int(c_3) << 6;	
		hex |= base64_to_int(c_4);
	
		uint8_t a_1 = (hex >> 16) & 0xFF;
		uint8_t a_2 = (hex >> 8) & 0xFF;
		uint8_t a_3 = hex & 0xFF;

		if (j < output_len) hex_block->buf[j++] = a_1;
		if (j < output_len) hex_block->buf[j++] = a_2;
		if (j < output_len) hex_block->buf[j++] = a_3;

		hex ^= hex;
	}

	return hex_block;
}

/* the buffer can have nulls in it, making debugging hard */
hexbuf_t *decodeB64_from_file(char *file_name)
{
	int fd;
	uint8_t *mem_file;
	hexbuf_t *hex_block;
	struct stat sb;

	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (fstat(fd, &sb) == -1)
	{
		fprintf(stderr, "Failed to stat file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((mem_file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == NULL)
	{
		fprintf(stderr, "Failed to mmap file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	hex_block = decodeB64(mem_file, sb.st_size);
	(void)munmap(mem_file, sb.st_size);
	(void)close(fd);

	return hex_block;
}
