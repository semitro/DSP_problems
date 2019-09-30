#include <stdio.h>
#include <stddef.h>
#include <math.h>

#include "filter.h"


void print_signal(Signal* s){
    printf("{");
    for(size_t i = 0; i < s->len; i++)
        printf("%.2f, ", s->data[i]);
    puts("}");

}

void print_filter(FIR_Filter *filter){
    printf("{");
    for (int i = 0; i < filter->len; i++) {
        printf("%.2f, ", filter->h[i]);
    }
    puts("}");
}

const double epsilon = 0.000001;
int feq(double a, double b){
    return fabs(a - b) < epsilon;
}

void test(signal_data_t *data, unsigned int sample_len,
          coeff_data_t  *h_values, unsigned int h_len,
          signal_data_t *expected_out)
{
    static unsigned int test_num = 0;
    puts("\n***************");
    printf("Test #%d\n", test_num);
    test_num++;

    printf("Filter = ");
    FIR_Filter filter;
    filter.h   = h_values;
    filter.len = h_len;
    print_filter(&filter);

    printf("Sample = ");
    Signal sample;
    sample.data = data;
    sample.len  = sample_len;
    print_signal(&sample);

    apply_FIR(&sample, &filter);

    printf("Out    = ");
    print_signal(&sample);

    for (int i = 0; i < sample_len; i++) {
        if(!feq(sample.data[i], expected_out[i])){
            printf("TEST NOT PASSED: %.10f != %.10f at %d\n",
                   sample.data[i], expected_out[i], i);
            return;
        }
    }

    puts("Test passed");
}

void test_trivial(){
    coeff_data_t    hh[]   = {1.};
    signal_data_t data[]   = {1., 2., 3., 4., 5.};
    signal_data_t result[] = {1., 2., 3., 4., 5.};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh,   sizeof (hh)/sizeof(coeff_data_t),
         result);
}

void test_zero(){
    coeff_data_t    hh[]   = {0., 0.};
    signal_data_t data[]   = {-1984.42, -3., 0., 3., 1984.42};
    signal_data_t result[] = {-1984.42,  0., 0., 0.,      0.};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh,   sizeof (hh)/sizeof(coeff_data_t),
         result);
}

void test_negative(){
    coeff_data_t    hh[]   = {0., -1.};
    signal_data_t data[]   = {-1984.42,  -3., 0.,  3., 1984.42};
    signal_data_t result[] = {1., -2., -3., 4., 5.};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh,   sizeof (hh)/sizeof(coeff_data_t),
         result);
}


void test_avg(){
    coeff_data_t    hh[]   = {1/3., 1/3., 1/3.};
    signal_data_t data[]   = {-1., 0., 1., 1.,   2.,   2.,   2., -4., -7.,  2.};
    signal_data_t result[] = {-1., 0., 0., 2/3., 4/3., 5/3., 2.,  0., -3., -3.};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh, sizeof (hh)/sizeof (coeff_data_t),
         result);
}

int main(){
    for(size_t i = 5; i >= 1; i--){
        printf("%d\n", i);
    }
    test_trivial();
    test_zero();
    test_negative();
    test_avg();
    return 0;
}
