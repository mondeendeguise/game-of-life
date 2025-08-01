#include "prng.h"

static unsigned long int next = 1;

void srand(unsigned int seed) {
	next = seed;
}

// glibc parameters
#define RAND_MODULUS    (RAND_MAX+1)
#define RAND_MULTIPLIER 1103515245
#define RAND_INCREMENT  12345
#define RAND_MASK       0x7fffffff // truncate most-significant-bit

int rand(void) {
	next = (((next * RAND_MULTIPLIER) + RAND_INCREMENT) % RAND_MODULUS) & RAND_MASK;
	return next;
}

float randf(void) {
	return (float) rand() / (float) RAND_MAX;
}
