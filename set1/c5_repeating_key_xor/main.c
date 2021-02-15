#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	char crypt[3] = "ICE";
	int fd = -1;
	char buf[BUFSIZ];
	uint64_t bytes;

	if (argc == 2)
	{
		if ((fd = open(argv[1], O_RDONLY)) == -1)
		{
			fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	while ((bytes = read(fd == -1 ? STDIN_FILENO : fd, buf, BUFSIZ)) > 0)
		for (uint64_t i = 0; i < bytes; ++i)
			printf("%02x", buf[i] ^ crypt[i % 3]);

	close(fd);
}
