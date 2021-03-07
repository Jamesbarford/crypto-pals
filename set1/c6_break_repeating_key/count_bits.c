#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 count the number of bits set in a number i.e 0x7 -> 111 -> 3
*/

int main(int argc, char **argv)
{
	uint64_t count = 0, n;

	if (argc != 2)
	{
		fprintf(stderr, "Unsigned integer to count bits of must be provided\n");
		exit(EXIT_FAILURE);
	}
	
	n = atoi(argv[1]);

	while (n != 0)
	{
		n &= (n-1);
		count++;
	}

	printf("count: %ld  0x%lX\n", count, count);
}
