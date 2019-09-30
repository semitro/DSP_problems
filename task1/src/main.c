#include "filter.h"
#include "stdio.h"

void print_signal(Signal* s){
    for(int i = 0; i < s->len; i++)
        printf("%.2f, ", s->data[i]);
    puts("");

}

int main(){
    Signal sample;
    signal_data_t data[] = {1., 1., 1., 1., 2., 2., 2., 2.,2., 2.};
    sample.data = data;
    sample.len  = 10;
    print_signal(&sample);

    FIR_Filter filter;
    coeff_data_t hh[] = {1/3., 1/3., 1/3.};
    filter.h = hh;
    filter.len = 3;

    apply_FIR(&sample, &filter);
    puts("After:"); 
    print_signal(&sample);
    return 0;
}
