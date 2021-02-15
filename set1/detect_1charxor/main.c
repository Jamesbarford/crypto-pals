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
	float letter_scoring;
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

    return frequency_table;
}

static struct Frequency *most_frequent(struct Frequency *frequency_table)
{
	struct Frequency *entry;
	float letter_scoring = 0.0;
	uint32_t idx = 0;

	if ((entry = malloc(sizeof(struct Frequency))) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for FrequencyEntry \n");
		exit(EXIT_FAILURE);
	}

	for (uint32_t i = 0; i < 0xFF; ++i)
	{
		if (frequency_table[i].letter_scoring > letter_scoring)
		{
			idx = i;
			letter_scoring = frequency_table[i].letter_scoring;
		}
	}

	entry->letter_scoring = frequency_table[idx].letter_scoring;
	entry->ascii_char = frequency_table[idx].ascii_char;

	return entry;
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

		if (entry->letter_scoring > current->letter_scoring)
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
