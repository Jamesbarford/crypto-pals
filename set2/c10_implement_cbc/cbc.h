#ifndef CBC_H
#define CBC_H

void cbc_decrypt(unsigned char *cipher_text, char *key, int cipher_len, char *iv);
void cbc_encrypt(unsigned char *plain_text, char *key, int cipher_len);

#endif
