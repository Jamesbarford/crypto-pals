#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

char *to_crack = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
char *valid = "";

struct Frequency {
	uint32_t ascii_char;
	uint32_t count;
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

static uint8_t most_frequent(struct Frequency *frequency_table)
{
	uint32_t count = 0;
	uint8_t current_char = 0;

	for (uint32_t i = 0; i < 0xFF; ++i)
	{
		struct Frequency freq = frequency_table[i];
		if (freq.count > count)
		{
			count = freq.count;
			current_char = freq.ascii_char;
		}
	}

	return current_char;
}

static struct Frequency *create_table()
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
		frequency_table[cur].count = 0;

		while ((pswd_c = to_crack[j++]) != '\0')
		{
			if (pos == 1)
			{
				cur_hex |= char_to_hex(pswd_c);
				unsigned char c = cur_hex ^ cur;
				if (tolower(c) >= 97 && tolower(c) <= 122)
					frequency_table[cur].count++;

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

	return frequency_table;
}

static void print_decrypted(uint8_t byte)
{
	char pswd_c;
	uint8_t pos = 0, hex = 0;
	uint32_t i = 0;

	while ((pswd_c = to_crack[i++]) != '\0')
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

int main(void)
{
	struct Frequency *frequency_table = create_table();	
	uint8_t most_common = most_frequent(frequency_table);
	print_decrypted(most_common);

	free(frequency_table);
}

