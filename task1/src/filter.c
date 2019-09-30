#include "filter.h"

void fir_simple(Signal *signal, FIR_Filter *filter){
    if(signal->len < filter->len){
        printf("filter::fir_simple: len of signal"
               " is less than len of filter!");
        return;
    }
    signal_data_t *data  = signal->data;
    const size_t h_num   = filter->len;
    coeff_data_t *h      = filter->h;// const!
    signal_data_t sum;
    size_t        h_index;
    const size_t  window_shift = h_num - 1; // We can't start to filter from the very beggining

/*
    for(size_t i = window:_shift; i < signal->len; i++){
        sum = 0;
        h_index = 0;
       for(size_t j = i - window_shift; j <= i; j++){
           sum += data[j]*h[h_index];
           h_index++;
       }
        data[i-window_shift] = sum;
    }

 */
    size_t i = signal->len;
    while(i-- != window_shift){
        sum = 0;
        h_index = 0;
       for(size_t j = i - window_shift; j <= i; j++){
           sum += data[j]*h[h_index];
          h_index++;
       }
       data[i] = sum;
    }
}

void apply_FIR(Signal *signal, FIR_Filter *filter){
    fir_simple(signal, filter);
}




