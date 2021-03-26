#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct Candidate {
	unsigned int repetitions;
	unsigned int line_no;
} Candidate;

static void detect_ecb(Candidate *c, char *text) {

}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int fd;
	char *file_name = argv[1];
	struct stat sb;

	if ((fd = open(file_name, O_RDONLY)) == -1) {
		fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (fstat(fd, &sb) == -1) {
		fprintf(stderr, "Failed to stat %s: %s\n", file_name, strerror(errno));
		(void)close(fd);
		exit(EXIT_FAILURE);
	}

	char *text = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
 	
	if (text == NULL) {
		fprintf(stderr, "Failed to mmap file %s: %s\n", file_name, strerror(errno));
		(void)close(fd);
		exit(EXIT_FAILURE);	
	}

	(void)munmap(text, sb.st_size);
	(void)close(fd);
}
