#include <stddef.h>
#include "test.h"
#include "filter.h"

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
    signal_data_t data[]   = {-1984.42,  -3., 0.,   3.,  1984.42};
    signal_data_t result[] = {-1984.42,   3., 0.,  -3., -1984.42};
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

void test_combined(){
    coeff_data_t    hh[]   = {-5., 10., 0.5, 1., 2.};
    signal_data_t data[]   = { 5., 2.5, 10, 0., 10.,   1.,   0.,  -1.,  -2.,   7};
    signal_data_t result[] = { 5., 2.5, 10, 0., 25., 99.5, -44., 98.5, -45., 6.5};

    test(data, sizeof(data)/sizeof(signal_data_t),
         hh, sizeof (hh)/sizeof (coeff_data_t),
         result);
}
/*
 * test below are crucial to check that xmm-routine handled correctly:
 * they check different combinations of data and hh sizes
 * (aliq is aliquot)
 */
void test_aliq4_h4(){
    coeff_data_t    hh[4]    = {1/4., 1/4., 1/4., 1/4.};
    signal_data_t data[12]   = { 0., 1., 2., 3.,   4.,  5.,  6.,  7.,  8.,  9., 10., 11.};
    signal_data_t result[12] = { 0., 1., 2., 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh, sizeof (hh)/sizeof (coeff_data_t),
         result);
}

void test_aliq4_h8(){
    coeff_data_t     hh[8]   = { 1.,  2.,  3., 4.,  5., 6., 7.,  8.};
    signal_data_t data[12]   = { 5., 2.5, 10., 0., 10., 1., 0., -1.,  1.,  1.,  1.,  1.};
    signal_data_t result[12] = { 5., 2.5, 10., 0., 10., 1., 0., 88., 68.5, 53., 39., 34.};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh, sizeof (hh)/sizeof (coeff_data_t),
         result);
}

void test_not_aliq_h7(){
    coeff_data_t       hh[7] =                                 { 1., 2.,  3., 4.,  5., 6., 7.};
    signal_data_t   data[13] = { 0., 1., -1., 2., -2., 3., -3.,  4.,  -4., 5.,  -5.,  6., 42.};
    signal_data_t result[13] = { 0., 1., -1., 2., -2., 3., -6., 22., -10., 26., -14., 30., 318.};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh, sizeof (hh)/sizeof (coeff_data_t),
         result);
}

void test_not_aliq_h9(){
    coeff_data_t     hh[9]   = { 1.,  2.,  3., 4.,  5., 6., 7.,  8., -8.};
    signal_data_t data[13]   = { 5., 2.5, 10., 0., 10., 1., 0., -1.,  1.,   1.,  1.,  1., 8.};
    signal_data_t result[13] = { 5., 2.5, 10., 0., 10., 1., 0., -1., 80., 60.5, 45., 31., -30.};
    test(data, sizeof(data)/sizeof(signal_data_t),
         hh, sizeof (hh)/sizeof (coeff_data_t),
         result);
}

long bench_avg(size_t signal_len, size_t filter_len){
    long res = benchmark(signal_len, filter_len);
    res += benchmark(signal_len, filter_len);
    res += benchmark(signal_len, filter_len);
    return res / 3;
}
/* test performance */
void benchmark_serial(){
    const size_t signal_len  = 1000000;
    for(size_t filter_len = 1; filter_len < 50; filter_len++){
        printf("%lu, ", bench_avg(signal_len, filter_len));
    }

    for(size_t filter_len = 50; filter_len < 100; filter_len +=5 ){
        printf("%lu, ", bench_avg(signal_len, filter_len));
    }
    puts("");
}

int main(int argc, char *argv[]){
    (void)argc; //needed to make `pedantic-errors` compiler flag patient
    (void)argv;
    puts("#############################################################");
    puts("Hello!");
    puts("This is implementation of FIR-filter made by Oshchepkov Artem");
    puts("Now I'm gonna run some tests to show that the program is correct");
    test_trivial();
    test_zero();
    test_negative();
    test_avg();
    test_combined();

    test_aliq4_h4();
    test_aliq4_h8();
    test_not_aliq_h7();
    test_not_aliq_h9();

    return 0;
}
