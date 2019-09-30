#include "filter.h"

void fir_simple(Signal *signal, FIR_Filter *filter){
    signal_data_t *data  = signal->data;
    const uint16_t h_num = filter->len;
    coeff_data_t *h      = filter->h;// const!

    signal_data_t sum;
    for(uint8_t i = h_num; i < signal->len; i++){
        sum = 0;
       for(uint8_t j = 0; j < h_num; j++){
           sum += data[i-j]*h[j];
       }
       data[i-h_num] = sum;
    }
}

void apply_FIR(Signal *signal, FIR_Filter *filter){
    fir_simple(signal, filter);
}




