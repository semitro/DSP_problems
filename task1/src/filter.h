#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

typedef float signal_data_t;

typedef struct Signal{
    signal_data_t* data;
    uint32_t len; 
}Signal;

typedef float coeff_data_t;

typedef struct FIR_Filter{
    coeff_data_t *h;
    uint16_t len;
}FIR_Filter;


void apply_FIR(Signal*, FIR_Filter*);

#endif // FILTER_H
