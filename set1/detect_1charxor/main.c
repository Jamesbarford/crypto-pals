/**
 * This is a bit grim as it requires 2 passes over the file
 *
 * Could store all wthe lines in memory, but then ALL the lines would be in memory
 * which seems far worse :/
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define HASH_TABLE_SIZE 2000

struct Frequency
{
	uint8_t ascii_char;
	uint32_t count;
	uint32_t readable;
};

struct EntryTable
{
	uint64_t size;
	struct Frequency *entries[];
};

struct Target
{
	uint64_t lineno;
	uint8_t ascii_char;
};

static uint8_t char_to_hex(char c);
static struct Frequency *most_frequent(struct Frequency *frequency_table);
static struct Frequency *create_table(char *encoded_str);
static struct Target *get_target(struct EntryTable *entry_table);
static void print_decrypted(char *str, uint8_t byte);
static void	second_pass(int fd, struct Target *target);
void read_file(const char *path);

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

static struct Frequency *most_frequent(struct Frequency *frequency_table)
{
	struct Frequency *entry;
	uint32_t count = 0, readable = 0;
	uint8_t ascii;

	if ((entry = malloc(sizeof(struct Frequency))) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for FrequencyEntry \n");
		exit(EXIT_FAILURE);
	}

	for (uint32_t i = 0; i < 0xFF; ++i)
	{
		struct Frequency freq = frequency_table[i];
		if (freq.readable > readable)
			readable = freq.readable;

		if (freq.count > count)
		{
			ascii = freq.ascii_char;
			count = freq.count;
		}
	}

	entry->readable = readable;
	entry->ascii_char = ascii;
	entry->count = count;

	return entry;
}

static struct Frequency *create_table(char *encoded_str)
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
		frequency_table[cur].readable = 0;

		while ((pswd_c = encoded_str[j++]) != '\0')
		{
			if (pos == 1)
			{
				cur_hex |= char_to_hex(pswd_c);
				unsigned char c = cur_hex ^ cur;
				// number of readable characters yeilded by the xor, i.e is this the most likely one in the file
				if (isalnum(c))
					frequency_table[cur].readable++;
				// character used to encode string
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

static struct Target *get_target(struct EntryTable *entry_table)
{
	struct Target *target;
	struct Frequency *entry, *current = entry_table->entries[0];

	if ((target = malloc(sizeof(struct Target))) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for Target\n");
		exit(EXIT_FAILURE);
	}

	for (uint64_t i = 0; i < entry_table->size; ++i)
	{
		entry = entry_table->entries[i];

		if (entry->readable > current->readable)
		{
			target->lineno = i;
			target->ascii_char = entry->ascii_char;
			current = entry;
		}
	}

	return target;
}

static void print_decrypted(char *str, uint8_t byte)
{
	char pswd_c;
	uint8_t pos = 0, hex = 0;
	uint32_t i = 0;

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

static void second_pass(int fd, struct Target *target)
{
	uint64_t bytes, pos = 0;
	char buf[BUFSIZ];
	char *str;
	uint64_t curline = 0;	

	if ((str = (char *)calloc(1000, 1)) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for string\n");
		exit(EXIT_FAILURE);
	}

	while ((bytes = read(fd, buf, BUFSIZ)) > 0)
	{
		for (uint64_t i = 0; i < bytes; ++i)
		{// we need to read only one line, but are blasting through the file again		
			if (curline == target->lineno)
			{
				if (buf[i] == '\n')
				{
					curline++;
					break;
				}
				else
					str[pos++] = buf[i];
			}
			if (buf[i] == '\n')
				curline++;
			if (curline > target->lineno)
				break;
		}
	}

	str[pos] = '\0';
	print_decrypted(str, target->ascii_char);
	free(str);
	free(target);
	close(fd);
}


void read_file(const char *path)
{
	int fd;
	uint8_t pos = 0;
	uint64_t bytes;
	char buf[BUFSIZ];
	char *current_line;
	struct EntryTable *entry_table;
	struct Frequency *frequency_table, *entry;

	if ((current_line = (char *)malloc(1000)) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for 'char *'\n");
		exit(EXIT_FAILURE);
	}

	if ((entry_table = (struct EntryTable *)malloc(sizeof(struct Frequency) * 500)) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for hashtable\n");
		exit(EXIT_FAILURE);
	}
	
	if ((fd = open(path, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Failed to open file: %s %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}

	entry_table->size = 0;
	memset(current_line, '\0', 1000);

	while ((bytes = read(fd, buf, BUFSIZ)) > 0)
	{
		for (uint64_t i = 0; i < bytes; ++i)
		{
			if (buf[i] == '\n')
			{
				current_line[pos] = '\0';
				frequency_table = create_table(current_line);
				entry = most_frequent(frequency_table);
				entry_table->entries[entry_table->size++] = entry;

				free(frequency_table);
				
				pos = 0;
				memset(current_line, '\0', 1000);
				continue;
			}
			current_line[pos++] = buf[i];
		}
	}

	frequency_table = create_table(current_line);
	entry = most_frequent(frequency_table);
	entry_table->entries[entry_table->size++] = entry;
	free(frequency_table);

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		fprintf(stderr, "Failed to lseek file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	second_pass(fd, get_target(entry_table));
}

int main(void)
{
	read_file("./challenge.txt");
}
