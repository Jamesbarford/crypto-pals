#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define ASCII_LIMIT 255

typedef struct Frequency {
	float letter_scoring;
} Frequency;

static inline unsigned char char_to_hex(char c) {
	char lower_char = tolower(c);
	if (lower_char >= 48 && lower_char <= 57)
		return lower_char - 48;

	else if (lower_char >= 97 && lower_char <= 122)
		return (lower_char - 'a') + 10;

	return 0;
}

// source http://en.wikipedia.org/Letter_Frequency
static inline float get_letter_frequency(unsigned char c) {
	switch (tolower(c)) {
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
		default:
			return -1.0;
	}
}

static unsigned char get_likely_byte(Frequency *freqencies) {	
	unsigned char likely_char = 0;
	float score = 0.0;

	for (unsigned char i = 0; i < ASCII_LIMIT; ++i) {
		Frequency freq = freqencies[i];
		if (freq.letter_scoring > score) {
			score = freq.letter_scoring;
			likely_char = i;
		}
	}

	return likely_char;
}

static void populate_frequencies(char *str, int len, Frequency *freqencies) {
	int j = 0;

	while (j < len) {
		int byte = 0;
		byte |= char_to_hex(str[++j]);
		byte |= char_to_hex(str[j-1]) << 4;
		freqencies[byte].letter_scoring += get_letter_frequency(byte);

		j++;
	}
}

static unsigned char get_xor_byte(char *str, int len) {
	Frequency freqencies[ASCII_LIMIT];

	for (int i = 0; i < ASCII_LIMIT; ++i)
		freqencies[i].letter_scoring = 0;

	populate_frequencies(str, len, freqencies);
	
	return get_likely_byte(freqencies);
}

void one_char_decrypt(char *text) {
	int len = strlen(text);
	int j = 0;
	unsigned char xor_byte = get_xor_byte(text, len);
	
	while (j < len) {
		int current = 0;
		current |= char_to_hex(text[++j]);
		current |= char_to_hex(text[j-1]) << 4;

		printf("%c", current ^ toupper(xor_byte));	
		j++;
	}
}

