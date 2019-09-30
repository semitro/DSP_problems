#include "filter.h"
#include <immintrin.h>

static inline void fir_simple(Signal *signal, FIR_Filter *filter){
    if(signal->len < filter->len){
        printf("filter::fir_simple: len of signal"
               " is less than len of filter!"); // where to include?
        return;
    }
    signal_data_t *data  = signal->data;
    const size_t h_num   = filter->len;
    coeff_data_t *h      = filter->h;// const!
    signal_data_t sum;
    size_t        h_index;
    const size_t  window_shift = h_num - 1; // We can't start to filter from the very beggining

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

#define XMM_REG_FLOAT_CAPACITY 4
static inline void fir_SIMD(Signal *signal, FIR_Filter *filter){
    signal_data_t *data  = signal->data;
    const size_t h_num   = filter->len;
    coeff_data_t *h      = filter->h;// const!
    signal_data_t sum;
    size_t        h_index;
    const size_t  window_shift = h_num - 1; // We can't start to filter from the very beggining

    __int128 *h_p   = (__int128*)h;
    __int128 *data_p = (__int128*)data;

    const size_t h_num_xmm             = filter->len / XMM_REG_FLOAT_CAPACITY;
    const size_t h_num_remainder       = filter->len / XMM_REG_FLOAT_CAPACITY;
    const size_t signal_len_xmm        = signal->len / XMM_REG_FLOAT_CAPACITY;
    const size_t signal_len_remainder  = signal->len / XMM_REG_FLOAT_CAPACITY;

    long i = 0;
    while(i < signal_len_xmm){
        i++;
        asm("movups %0, %%xmm0\n" : :"m"(*data_p) ); //xmm0 = {sample[n], sample[n+1], sample[n+2], sample[n+3]}
        for(size_t j = 0; j < h_num_xmm; j++){
            asm("mulps %0, %%xmm0\n" : :"m"(*h_p)); // xmm0 = {sample[n] * h[k], .., sample[n+3] * h[k+3]}
            h_p++;
        }
        data_p++;
    }
/*
    while(i > window_shift){

        asm("movups %0, %%xmm0\n" : :"m"(*data_p) ); //xmm0 = {sample_i, sample_i+1, sample_i+2, sample_i+3}
        sum = 0;
        h_index = 0;
        for(size_t j = i - window_shift; j <= i; j += xmm_corrective){
            asm("mulps %0, %%xmm0\n" : :"m"(*h_p)); // xmm0 = {sample_i * h_k, .., sample_i+3 * h_k+3}
            sum += data[j]*h[h_index];
            h_index++;
            if(h_index > 4)
                h_p++;
       }
       data[i] = sum;
    }
    */
}

void apply_FIR(Signal *signal, FIR_Filter *filter){
    fir_simple(signal, filter);
}




