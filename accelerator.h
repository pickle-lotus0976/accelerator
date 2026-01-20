#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include "ap_int.h"
#include "hls_stream.h"

#define N 10
#define PAR_FACTOR 4

typedef ap_int<12> data_t;
typedef ap_int<16> result_t;

void weighted_sum_accelerator(data_t *input_mem, result_t *output_mem);

#endif