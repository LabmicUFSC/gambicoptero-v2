/*
 * random.c
 *
 *  Created on: Jun 24, 2010
 *      Author: mateus
 */


#include <utility/random.h>
#include "../include/abst_random.h"

#define __RANDOM_DETAILED_TRACE 0

#define RAND_MAX 32767

static System::Pseudo_Random random;

void random_set_seed(int seed) {
	//srand(seed);
	random.seed(seed);
}

int random_give_int_smaller_than(int hi) {
/* Adapted from: http://www.cppreference.com/wiki/c/other/rand */

	// scale in range [0,1)
	//int r = rand();
	int r = (int) random.random();

	const float scale = r / ((float) RAND_MAX);

#if __RANDOM_DETAILED_TRACE
		printf("...r:%i\n", r);
		printf("...scale:%f\n", scale);
		printf("... (int) scale:%i\n", (int) (scale * (float) hi));
#endif

	// return range [0..hi-1]
	return ((int) (scale * (float) hi)); // implicit cast and truncation in return
}

int random_give_non_zero_int_smaller_than(int hi) {
	int i = 0;

	while (i == 0) {
		i = random_give_int_smaller_than(hi);

#if __RANDOM_DETAILED_TRACE
		printf("help, I traped here!");
#endif
	}

	return i;
}

int random_give_int_multiple_of(int n) {
	return n * random.random();
}

/* Returns a non zero multiple of n, up to m.  */
int random_give_int_multiple_of_n_up_to_m(int n, int m) {
	float x = ((float) m) / ((float) n);
	int y = random_give_int_smaller_than((int) x + 1);

	if (y == 0) {
		y = 1;
	}

	//assert(y * n <= m);

	return y * n;
}
