#include <stdio.h>
#include <openssl/err.h>

#include "encrypt_errors.h"

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

