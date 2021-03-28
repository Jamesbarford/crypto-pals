#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "cbc.h"

/* make function from previous challenge a util  */
void pad(char *text) {
	printf("%s\n", text);
}

/*i 32bit Circular left rotation https://www.aldeid.com/wiki/X86-assembly/Instructions/rol */
static int inline rot_l(uint32_t value, unsigned int r) {
	__asm__("roll %1, %0" : "+g" (value) : "cI" ((unsigned char) r));
	return value;
}

/* Given a plain text key create round keys, https://en.wikipedia.org/wiki/AES_key_schedule think thats a good
 * starting place to find info
 * */
static void inline expand_key(unsigned char *key);

/* For decryption we need an inverse sbox: https://en.wikipedia.org/wiki/Rijndael_S-box */
void cbc_decrypt(unsigned char *cipher_text, char *key, int cipher_len, char *iv) {
	int cipher_text_len = strlen((char *)cipher_text);
	if (cipher_len % 16 != 0) {
		// add padding
	}

	/*
	 * char buf[cipher_len + padding];
	 * unsigned char Round;
	 * expand_key(key)
	 * */
}


void cbc_encrypt(unsigned char *plain_text, char *key, int cipher_len);

