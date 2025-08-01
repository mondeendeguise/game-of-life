#ifndef _PRGN_H
#define _PRGN_H

#define RAND_MAX 2147483648 // 2^31

extern int time(void);

int rand(void);
void srand(unsigned int seed);

float randf(void);

#endif // _PRGN_H
