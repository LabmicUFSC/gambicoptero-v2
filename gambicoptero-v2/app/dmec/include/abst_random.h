/*
 * random.h
 *
 *  Created on: Jun 24, 2010
 *      Author: mateus
 */

#ifndef RANDOM_H_
#define RANDOM_H_

void random_set_seed(int seed);

int random_give_int_smaller_than(int hi);

int random_give_non_zero_int_smaller_than(int hi);

int random_give_int_multiple_of(int n);

int random_give_int_multiple_of_n_up_to_m(int n, int m);

#endif /* RANDOM_H_ */
