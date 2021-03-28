#include <stdio.h>

#include "../../includes/readinfile/readinfile.h"
#include "../../includes/base64tohex/base64tohex.h"
#include "cbc.h"

int main(int argc, char **argv) {
	mmapd_file_t mmapd_file;
	char *file_name = argv[1];
	char *iv = "\0\0\0";
	char *key = "YELLOW SUBMARINE";
	
	read_file(file_name, &mmapd_file);
	hexbuf_t *hexbuf = decodeB64((unsigned char *)mmapd_file.text, mmapd_file.size+1); 
	
	cbc_decrypt(hexbuf->buf, key, 10, iv);

	free_hex_buf(hexbuf);
	free_file(&mmapd_file);
}

