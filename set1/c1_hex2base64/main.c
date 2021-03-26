#include "../../includes/readinfile/readinfile.h"
#include "hex2base64.h"

int main(int argc, char **argv) {
	switch (argc) {
		case 1: read_stdin(to_base64); break;
		case 2: read_exec(argv[1], to_base64); break;
		default:
			exit(EXIT_FAILURE);
			return -1;
	}

	printf("\n");
	exit(EXIT_SUCCESS);
}
