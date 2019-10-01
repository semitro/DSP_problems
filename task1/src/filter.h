#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stddef.h>

//#ifdef USE_SSE
#include <mmintrin.h>
#include <xmmintrin.h>
//CPUID Flags: SSE3
#include <pmmintrin.h>
//#endif

typedef float signal_data_t;

typedef struct Signal{
    signal_data_t* data;
    size_t len;
}Signal;

typedef float coeff_data_t;

typedef struct FIR_Filter{
    coeff_data_t *h;
    size_t len;
}FIR_Filter;


void apply_FIR(Signal*, FIR_Filter*);

#endif // FILTER_H
