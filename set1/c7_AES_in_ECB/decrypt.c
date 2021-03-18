#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "decrypt.h"
#include "encrypt_errors.h"

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{

	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();

	/*
	 * Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv) != 1)
		handleErrors();

	/*
	 * Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary.
	 */
	if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
		handleErrors();
	plaintext_len = len;

	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
		handleErrors();
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}
