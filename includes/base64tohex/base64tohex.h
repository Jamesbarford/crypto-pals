#ifndef BASE64_TO_HEX
#define BASE64_TO_HEX

#include <stdint.h>
#include <stddef.h>

typedef struct hexbuf_t
{
	size_t size;
	uint8_t buf[];
} hexbuf_t;

hexbuf_t *decodeB64(uint8_t *b64, size_t input_len);
hexbuf_t *decodeB64_linebreaks(uint8_t *b64, size_t input_len);
hexbuf_t *decodeB64_from_file(char *file_name);
void free_hex_buf(hexbuf_t *hexbuf);

#endif
