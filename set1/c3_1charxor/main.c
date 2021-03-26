#include "../../includes/readinfile/readinfile.h"
#include "1charxor.h"

int main(int argc, char **argv) {
	switch (argc) {
		case 1: read_stdin(one_char_decrypt); break;
		case 2: read_exec(argv[1], one_char_decrypt); break;
		default:
			exit(EXIT_FAILURE);
			return -1;
	}

	printf("\n");
	exit(EXIT_SUCCESS);
}


