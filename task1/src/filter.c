#include "filter.h"

#ifdef USE_SSE
#define XMM_SIZE 4 /* how many floats can be stored in xmm register */
typedef union mem128_bulk {__m128 m; float f[XMM_SIZE];}mem128_bulk;

static inline void fir_SIMD(Signal *signal, const FIR_Filter *filter){
    const size_t window_shift = filter->len - 1; // We can't start to filter from the very beggining
    const size_t h_remainder = filter->len % XMM_SIZE; // how many elements we need to count without SIMD

    __m128 h_vector;    // xmm register which contains filter coefficients
    __m128 data_vector; // xmm for data
    signal_data_t sum = 0.f;
    size_t j;
    size_t h_index; // which filter coefficient is used at the moment
    size_t i = signal->len; // we're moving from the end to the beginning

    while(i-- != window_shift){ // don't process several first samples
        sum = 0;
        h_index = 0;
        for(j = i - window_shift; j + h_remainder < i;
                    j+= XMM_SIZE, h_index += XMM_SIZE){// for each sample but not 1, 2 or 3 last ones
            data_vector = _mm_loadu_ps(signal->data + j);// d_v= {sample[n-3], sample[n-2], sample[n-1], sample[n]}
            h_vector    = _mm_loadu_ps(filter->h + h_index);// h_v = {h[k-3], .., h[k]
            data_vector = _mm_mul_ps(data_vector, h_vector);// d_v = {sample[n-3] * h[k-3], .., sample[n] * h[k]}
            data_vector = _mm_hadd_ps(data_vector, data_vector); // d_v  = { half of horizontal summ, ... }
            data_vector = _mm_hadd_ps(data_vector, data_vector); // d_v[0] = horizontal sum (sum of sample[n-i]*h[k-i])
            sum += ((mem128_bulk*)&data_vector)->f[0]; // we got sum for elements except several leftovers
        }
        // count the leftovers
        for( size_t k = 0; k < h_remainder; k++){
          sum += signal->data[j]*filter->h[h_index];
          h_index++;
          j++;
        }
       signal->data[i] = sum;
    }
}
#else /*if USE_SSE is not define */
static inline void fir_simple(Signal *signal, const FIR_Filter *filter){
    const size_t  window_shift = filter->len - 1; // We can't start to filter from the very beggining
    signal_data_t sum = 0.f;
    size_t h_index = 0;
    size_t i = signal->len;

    while(i-- != window_shift){ // from the end to the beggining
        sum = 0;
        h_index = 0;
       for(size_t j = i - window_shift; j <= i; j++){
           sum += signal->data[j]*filter->h[h_index];
          h_index++;
       }
       signal->data[i] = sum;
    }
}

#endif

void apply_FIR(Signal *signal, const FIR_Filter *filter){
    if(signal->len == 0 || filter->len == 0){
        return;
    }
    if(signal->len < filter->len){
        printf("filter::fir_simple: len of signal"
               " is less than len of filter!"); // where to include?
        return;
    }
#ifdef USE_SSE
    fir_SIMD(signal, filter);
#else
    fir_simple(signal, filter);
#endif
}

