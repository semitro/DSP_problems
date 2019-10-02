#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "filter.h"

/* pretty print functions */
void print_signal(const Signal* s);
void print_filter(const FIR_Filter *filter);

/* float equals precise */
#define EPSILON 0.000001f;
/*float equals */
int feq(float a, float b);

/* check if data processed by h coefficients matches exptected out
 * and print report */
void test(signal_data_t *data, unsigned int sample_len,
          coeff_data_t *h_values, unsigned int h_len,
          const signal_data_t *expected_out);

time_t benchmark(size_t sample_len, size_t filter_len);

#endif // TEST_T
