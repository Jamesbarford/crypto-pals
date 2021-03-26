#include <stdio.h>
#include <string.h>

int main(void) {
	int block_size = 20;
	char *text = "YELLOW SUBMARINE";
	int text_len = strlen(text);
	int padding_len = block_size - (text_len % block_size);
	int new_length = padding_len + text_len + 1;

	char text_with_padding[new_length];
	char padding[padding_len];
	memset(padding, 'F', padding_len);

	strcpy(text_with_padding, text);
	strcat(text_with_padding, padding);
	text_with_padding[new_length] = '\0';

	printf("Padded text: %s\n", text_with_padding);
}
