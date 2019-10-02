#include "test.h"

void print_signal(const Signal* s){
    printf("{");
    for(size_t i = 0; i < s->len; i++)
        printf("%.2f, ", s->data[i]);
    puts("}");

}

void print_filter(const FIR_Filter *filter){
    printf("{");
    for (size_t i = 0; i < filter->len; i++) {
        printf("%.2f, ", filter->h[i]);
    }
    puts("}");
}

int feq(float a, float b){
    return fabs(a - b) < EPSILON;
}

void test(signal_data_t *data, unsigned int sample_len,
          coeff_data_t  *h_values, unsigned int h_len,
          const signal_data_t *expected_out)
{
    static unsigned int test_num = 1;
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

    for (size_t i = 0; i < sample_len; i++) {
        if(!feq(sample.data[i], expected_out[i])){
            printf("TEST NOT PASSED: %.10f != %.10f at %lu\n",
                   sample.data[i], expected_out[i], i);
            return;
        }
    }

    puts("Test passed");
}


time_t benchmark(size_t sample_len, size_t filter_len){
    Signal sample;
    sample.len  = sample_len;
    sample.data = malloc(sample_len * sizeof(signal_data_t));
    for (size_t i = 0; i < sample.len; i++){
        sample.data[i] = (i % 2 == 0? (float)i : -(float)i);
    }

    FIR_Filter filter;
    filter.len  = filter_len;
    filter.h    = malloc(filter_len* sizeof(coeff_data_t));
    for (size_t i = 0; i < filter.len; i++){
        filter.h[i] = (i % 2 == 0? (float)i : -(float)i);
    }
    struct rusage r;
    struct timeval start;
    struct timeval end;
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;
    apply_FIR(&sample, &filter);
    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;
    long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
    free(filter.h);
    free(sample.data);
    return res;
}
