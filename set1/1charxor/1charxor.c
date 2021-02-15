#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

struct Frequency
{
	uint32_t ascii_char;
	float letter_scoring;
};

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

// source http://en.wikipedia.org/Letter_Frequency
static float get_letter_frequency(uint8_t c)
{
	switch(tolower(c))
	{
		case 'a': return 8.2;
		case 'b': return 1.5;
		case 'c': return 2.8;
		case 'd': return 4.3;
		case 'e': return 13.0;	
		case 'f': return 2.2;
		case 'g': return 2.0;
		case 'h': return 6.1;
		case 'i': return 7.0;
		case 'j': return 0.15;
		case 'k': return 0.77;
		case 'l': return 4.0;
		case 'm': return 2.4;
		case 'n': return 6.7;
		case 'o': return 7.5;
		case 'p': return 1.9;
		case 'q': return 0.095;
		case 'r': return 6.0;
		case 's': return 6.3;
		case 't': return 9.1;
		case 'u': return 2.8;
		case 'v': return 0.98;
		case 'w': return 2.4;
		case 'x': return 0.15;
		case 'y': return 2.0;
		case 'z': return 0.074;
		default:
			return -1.0;
	}
}

static uint8_t byte_used(struct Frequency *frequency_table)
{
	float score = 0.0;
	uint32_t idx = 0;
	for (uint32_t i = 0; i < 0xFF; ++i)
	{
		struct Frequency freq = frequency_table[i];
		if (freq.letter_scoring > score)
		{
			idx = i;
			score = freq.letter_scoring;
		}
	}

	return freqquency_table[idx].ascii_char;
}

static struct Frequency *create_table(char *str)
{
	char pswd_c;
	uint32_t cur = 0, j = 0;
	uint8_t cur_hex = 0, pos = 0;
	struct Frequency *frequency_table;

	if ((frequency_table = malloc(sizeof(struct Frequency) * 0xFF)) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for frequency table: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while (cur != 0xFF)
	{
		frequency_table[cur].ascii_char = cur;
		frequency_table[cur].letter_scoring = 0;

		while ((pswd_c = str[j++]) != '\0')
		{
			if (pos == 1)
			{
				cur_hex |= char_to_hex(pswd_c);
				uint8_t c = cur_hex ^ cur;
				frequency_table[cur].letter_scoring += get_letter_frequency(c);

				pos = 0;
				cur_hex = 0;
			}
			else
			{
				cur_hex |= char_to_hex(pswd_c) << 4;
				pos++;
			}
		}
		j = 0;
		cur++;
	}

	print_s(frequency_table);
	return frequency_table;
}

static void print_decrypted(char *str, uint8_t byte)
{
	char pswd_c;
	uint8_t pos = 0, hex = 0;
	uint32_t i = 0;
	printf("0x%X\n", byte);
	while ((pswd_c = str[i++]) != '\0')
	{
		if (pos == 1)
		{
			hex |= char_to_hex(pswd_c);
			printf("%c", hex ^ byte);
			hex = 0;
			pos = 0;
		}
		else
		{
			hex |= char_to_hex(pswd_c) << 4;
			pos++;
		}
	}

	printf("\n");
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Must provide hex string argument\n");
		exit(EXIT_FAILURE);
	}

	struct Frequency *frequency_table = create_table(argv[1]);	
	uint8_t most_common = byte_used(frequency_table);
	print_decrypted(argv[1], most_common);

	free(frequency_table);
}

