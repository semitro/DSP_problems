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
    signal_data_t *data_p  = signal->data;
    const size_t h_num   = filter->len;
    coeff_data_t *h      = filter->h;// const!
    size_t        h_index;
    const size_t  window_shift = h_num - 1; // We can't start to filter from the very beggining

    typedef union mem128_bulk {__m128 m; float f[XMM_REG_FLOAT_CAPACITY];}mem128_bulk;
    __m128 *h_p128 = (__m128*)filter->h;
    __m128 *data_p128 = (__m128*)signal->data;

    const size_t h_num_xmm             = filter->len / XMM_REG_FLOAT_CAPACITY;
    const size_t h_num_remainder       = filter->len / XMM_REG_FLOAT_CAPACITY;
    const size_t signal_len_xmm        = signal->len / XMM_REG_FLOAT_CAPACITY;
    const size_t signal_len_remainder  = signal->len / XMM_REG_FLOAT_CAPACITY;
    const size_t signal_len_adjusted   = signal->len - signal->len % XMM_REG_FLOAT_CAPACITY;
    __m128 h_vector;
    __m128 data_vector;
    /*
    signal_data_t sum = 0.f;
    for(size_t i = 0; i < signal_len_adjusted; i++){ // for each sample but not 1, 2 or 3 last ones
        data_p++; // point to very next sample
        data_p128 = (__m128*)(data_p - h_num - 1);  // point to beginning of the window
        for(size_t j = 0; j < h_num_xmm; j++){ 		 			 // process bulks of 4 floats
            data_vector = _mm_loadu_ps((float*)data_p128); 	     // d_v= {sample[n-3], sample[n-2], sample[n-1], sample[n]}
            h_vector    = _mm_loadu_ps((float*)h_p128);    	     // h_v = {h[k-3], .., h[k]
            data_vector = _mm_mul_ps(data_vector, h_vector); 	 // d_v = {sample[n-3] * h[k-3], .., sample[n] * h[k]}
            data_vector = _mm_hadd_ps(data_vector, data_vector); // d_v  = { half of horizontal summ, ... }
            data_vector = _mm_hadd_ps(data_vector, data_vector); // d_v[0] = horizontal sum (sum of sample[n-i]*h[k-i])
            sum = ((mem128_bulk*)&data_vector)->f[0]; // we got almost Xi using not all of the coefficients of the filter
            h_p128++;
            data_p128++;
        }
       // mem128_bulk *data_remainder = (mem128_bulk*)data_p128;
       // mem128_bulk *h_remainder    = (mem128_bulk*)h_p128;
       // for(size_t j_r = 0; j_r < h_num_remainder; j_r++){
       //     remainder_sum += data_remainder->f[j_r]*h_remainder->f[j_r];
       // }
    }*/
    //data_p = (signal->data + signal->len - 1); // point to last element
    size_t i = signal->len;
    signal_data_t sum = 0.f;
    while(i-- != window_shift){
        sum = 0;
        h_index = 0;
       for(size_t j = i - window_shift; j <= i; j+= 4){
            data_vector = _mm_loadu_ps(signal->data + j);
            h_vector    = _mm_loadu_ps(filter->h + h_index);
            data_vector = _mm_mul_ps(data_vector, h_vector);
            data_vector = _mm_hadd_ps(data_vector, data_vector); // d_v  = { half of horizontal summ, ... }
            data_vector = _mm_hadd_ps(data_vector, data_vector); // d_v[0] = horizontal sum (sum of sample[n-i]*h[k-i])
            sum += ((mem128_bulk*)&data_vector)->f[0]; // we got almost Xi using not all of the coefficients of the filter
            h_index += 4;
       }
       data_p[i] = sum;
    }
}


void apply_FIR(Signal *signal, FIR_Filter *filter){
    fir_SIMD(signal, filter);
    //fir_simple(signal, filter);
}




