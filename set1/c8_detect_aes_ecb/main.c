#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../includes/readinfile/readinfile.h"

static int detect_ecb_mode(mmapd_file_t *mmapd_file, unsigned int key_length) {
	unsigned int line_len = get_line_len(mmapd_file);
	unsigned int blocks_size = line_len / key_length;
	char cur_line[line_len];
	char block1[key_length + 1];
	char block2[key_length + 1];
	memset(cur_line, '\0', line_len);

	for (unsigned int lineno = 0; lineno < mmapd_file->size / line_len; ++lineno) {
		memcpy(cur_line, &mmapd_file->text[(lineno * line_len) + lineno], line_len);
		cur_line[line_len] = '\0';

		for (unsigned int j = 0; j < blocks_size; ++j) {
			memcpy(block1, &cur_line[j * key_length], key_length);
			block1[key_length] = '\0';

			for (unsigned int k = j + 1; k < blocks_size; ++k) {
				memcpy(block2, &cur_line[k * key_length], key_length);
				block2[key_length] = '\0';

				printf("b1: %s, b2: %s\n", block1, block2);
				if (strcmp(block1, block2) == 0) {
					return lineno + 1; // lines in a file are not zero based
				}
			}
		}	
	}

	return -1;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mmapd_file_t mmapd_file;

	read_file(argv[1], &mmapd_file);
	int line = detect_ecb_mode(&mmapd_file, 16);
	printf("line: %d\n", line);
	free_file(&mmapd_file);
}
