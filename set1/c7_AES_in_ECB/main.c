#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "../../includes/base64tohex/base64tohex.h"
#include "decrypt.h"

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		fprintf(stderr, "Usage: ./decrypt.out <file_name>\n");
		exit(EXIT_FAILURE);
	}

	// in the real world these would not be hard coded
	unsigned char *cipherkey = (unsigned char *)"YELLOW SUBMARINE";
	unsigned char *iv = (unsigned char *)"0123456789012345";	
	int fd;
	char *file_name = argv[1];
	struct stat sb;
	hexbuf_t *hex_buf = decodeB64_from_file(file_name);

	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Failed to open '%s': %s\n", file_name, strerror(errno));
		exit(EXIT_FAILURE);	
	}

	if (fstat(fd, &sb) == -1)
	{
		fprintf(stderr, "Failed to stat: '%s': %s\n", file_name, strerror(errno));
		exit(EXIT_FAILURE);	
	}

	unsigned char buf[sb.st_size];
	
	decrypt(hex_buf->buf, hex_buf->size, cipherkey, iv, buf);

	buf[hex_buf->size] = '\0';
	printf("%s", buf);

	(void)close(fd);
	(void)free(hex_buf);
}
