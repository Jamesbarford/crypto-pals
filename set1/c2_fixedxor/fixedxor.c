// takes-> 																			1c0111001f010100061a024b53535009181c
// after hex decoding, and when XOR'd against		686974207468652062756c6c277320657965
// produces 																		746865206b696420646f6e277420706c6179

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

char *input = "1c0111001f010100061a024b53535009181c";
char *xor 	= "686974207468652062756c6c277320657965";

static uint8_t char_to_hex(char c)
{
	char lower_char = tolower(c);
	if (lower_char >= 48 && lower_char <= 57)
		return lower_char - 48;

	else if (lower_char >= 97 && lower_char <= 122)
		return (lower_char - 'a') + 10;

	printf("'%d' is not a valid hex value\n", c);
	exit(EXIT_FAILURE);
	return 0;
}

int main(void)
{
	char c;
	uint64_t ptr = 0;
	while ((c = input[ptr++]) != '\0')
		printf("%x", char_to_hex(xor[ptr - 1]) ^ char_to_hex(c));

	exit(EXIT_SUCCESS);
}
