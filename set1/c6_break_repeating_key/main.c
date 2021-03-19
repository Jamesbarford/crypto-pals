/**
 * This implementation cannot handle a base64 file with new line separators
*/
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "../../includes/base64tohex/base64tohex.h"

// source https://en.wikipedia.org/wiki/Letter_frequency
static inline float get_letter_frequency(unsigned char c)
{
	switch(tolower(c))
	{
		case 'a': return 8.167;
		case 'b': return 1.492;
		case 'c': return 2.782;
		case 'd': return 4.253;
		case 'e': return 12.702;
		case 'f': return 2.228;
		case 'g': return 2.015;
		case 'h': return 6.094;
		case 'i': return 6.966;
		case 'j': return 0.153;
		case 'k': return 0.772;
		case 'l': return 4.025;
		case 'm': return 2.406;
		case 'n': return 6.749;
		case 'o': return 7.507;
		case 'p': return 1.929;
		case 'q': return 0.095;
		case 'r': return 5.987;
		case 's': return 6.327;
		case 't': return 9.056;
		case 'u': return 2.758;
		case 'v': return 0.978;
		case 'w': return 2.36;
		case 'x': return 0.15;
		case 'y': return 1.974;
		case 'z': return 0.074;
		case ' ': return 14.0;
		default:
			return -1.0;
	}
}

size_t hamming_distance(char *str1, char *str2, size_t len)
{
	size_t acc = 0;

	for (size_t i = 0; i < len; ++i)
		acc += __builtin_popcount(str1[i] ^ str2[i]);

	return acc;
}

/* key size with the smallest edit distance is the key */
size_t get_key_size(hexbuf_t *hex_buf)
{
	uint64_t bytes = 0, key_size = 0;
	float unlikely = (float)UINT64_MAX;

	for (uint8_t i = 2; i <= 40; ++i)
	{
		float normalise = 0.0;
		bytes = hex_buf->size / i;

		for (uint64_t j = 0; j < bytes; ++j)
		{
			char subbufA[i];
			char subbufB[i];
			memcpy(subbufA, &hex_buf->buf[j * i], i);
			memcpy(subbufB, &hex_buf->buf[(j + 1) * i], i);

			uint64_t edit_distance = hamming_distance(subbufA, subbufB, i);

			normalise += (float)edit_distance / (float)i;
		}

		normalise /= bytes;

		if (normalise > 0 && normalise < unlikely)
		{
			unlikely = normalise;
			key_size = i;
		}
	}

	return key_size;
}

static unsigned char brute_force_single_char_xor(char *cipher_text, uint64_t len)
{
	unsigned char likely_char = 0;
	float score = 0.0;

	for (uint16_t c = 0; c <= 255; ++c)
	{
		char buf[len];
		float new_score = 0.0;

		for (uint64_t j = 0; j < len; ++j)
		{
			buf[j] = cipher_text[j] ^ c;
			new_score += get_letter_frequency(buf[j]);
		}

		if (new_score > score)
		{
			score = new_score;
			likely_char = c;
		}
	}

	return likely_char;
}

unsigned char *get_key(size_t key_length, hexbuf_t *hex_buf)
{
	size_t block_size = hex_buf->size / key_length;
	unsigned char *key;

	if ((key = (unsigned char *)malloc(sizeof(unsigned char) * key_length)) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for key\n");
		exit(ENOMEM);
	}

	for (size_t i = 0; i < key_length; ++i)
	{
		uint64_t ptr = 0;
		char block[block_size];

		for (size_t j = 0; j < block_size; ++j) 
		{
			size_t idx = (j * key_length) + i;
			unsigned char c = hex_buf->buf[idx];
			block[ptr++] = c;
		}

		key[i] = brute_force_single_char_xor(block, ptr);
	}

	return key;
}

unsigned char *repeating_key_xor(hexbuf_t const *hex_buf, unsigned char const *key, size_t const key_size)
{
	unsigned char *decoded;

	if ((decoded = (unsigned char *)malloc(sizeof(unsigned char) * hex_buf->size)) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for decoded string\n");
		exit(ENOMEM);
	}

	for (size_t i = 0; i < hex_buf->size - 1; ++i)
		decoded[i] = (hex_buf->buf[i] ^ key[i % key_size]);

	return decoded;
}

int main(void)
{
	char *file_name = "./file_no_newlines.txt";
	hexbuf_t *hex_buf = decodeB64_from_file(file_name);
	size_t key_size = get_key_size(hex_buf); 
	unsigned char *key = get_key(key_size, hex_buf);
	unsigned char *decoded = repeating_key_xor(hex_buf, key, key_size);

	printf("Key size: %ld\t Key: %s\n", key_size, key);
	printf("%s\n", decoded);

	free(hex_buf);
	free(key);
	free(decoded);
}
