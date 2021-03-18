#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		fprintf(stderr, "Usage: ./decrypt.out <file_name>\n");
		exit(EXIT_FAILURE);
	}

	int fd;
	char *file_name = argv[1];
	struct stat sb;

	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Failed to open '%s': %s\n", file_name, strerror(errno));
		exit(EXIT_FAILURE);	
	}

	if (fstat(fd, &sb) == -1)
	{
		fprintf(stderr, "Failed to stat: '%s': %s\n", file_name, strerror(errno));
		goto failed;
	}

	char *full_file =  mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (full_file == NULL)
	{
		fprintf(stderr, "Failed to mmap: '%s': %s\n", file_name, strerror(errno));
		goto failed;
	}
	printf("%s", full_file);
	(void)munmap(full_file, sb.st_size);

failed:
	exit(EXIT_FAILURE);
	(void)close(fd);
}
