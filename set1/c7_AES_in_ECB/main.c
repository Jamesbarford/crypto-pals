#include <stdio.h>
#include <stdlib.h>

#include "../../includes/base64tohex/base64tohex.h"
#include "decrypt.h"

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		fprintf(stderr, "Usage: ./decrypt.out <file_name>\n");
		exit(EXIT_FAILURE);
	}

	char *file_name = argv[1];
	hexbuf_t *hex_buf = decodeB64_from_file(file_name);
	unsigned char buf[hex_buf->size];

	// in the real world these would not be hard coded
	unsigned char *cipherkey = (unsigned char *)"YELLOW SUBMARINE";
	unsigned char *iv = (unsigned char *)"0123456789012345";	
	
	decrypt(hex_buf->buf, hex_buf->size, cipherkey, iv, buf);

	buf[hex_buf->size] = '\0';
	printf("%s", buf);

	(void)free(hex_buf);
}
