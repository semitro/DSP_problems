#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
/* USE_SSE macro determines if we use simd instructions or not */
#ifdef USE_SSE
#include <mmintrin.h>
#include <xmmintrin.h>
#include <x86intrin.h>
#endif

typedef float signal_data_t;
typedef float coeff_data_t;

typedef struct Signal{
    signal_data_t* data; /* Signal samples array */
    size_t len;			 /* Samples num */
}Signal;


typedef struct FIR_Filter{
    coeff_data_t *h; /* Impulse response array */
    size_t len;		 /* Impulses num */
}FIR_Filter;

/* transform source signal by filter */
void apply_FIR(Signal*, const FIR_Filter*);

#endif // FILTER_H
