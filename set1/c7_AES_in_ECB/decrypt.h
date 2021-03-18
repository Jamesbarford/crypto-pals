#ifndef DECRYPT_H
#define DECRYPT_H

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

#endif
