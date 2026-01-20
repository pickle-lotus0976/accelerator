#include "accelerator.h"

static const data_t weights[N] = {1, 2, 3, 4, 5, 6, 8, 9, 7, 1};

void read_data(data_t *in_mem, hls::stream<data_t> &in_stream) {
    for (int i = 0; i < N; i++) {
        #pragma HLS PIPELINE II=1
        in_stream.write(in_mem[i]);
    }
}

void compute(hls::stream<data_t> &in_stream, hls::stream<result_t> &out_stream) {
    #pragma HLS ARRAY_PARTITION variable=weights dim=1 type=cyclic factor=4
    for (int i = 0; i < N; i++) {
        #pragma HLS PIPELINE II=1
        data_t val = in_stream.read();
        result_t res = val * weights[i];
        out_stream.write(res);
    }
}

void write_data(hls::stream<result_t> &out_stream, result_t *out_mem) {
    for (int i = 0; i < N; i++) {
        #pragma HLS PIPELINE II=1
        out_mem[i] = out_stream.read();
    }
}

void weighted_sum_accelerator(data_t *input_mem, result_t *output_mem) {
    #pragma HLS INTERFACE m_axi port=input_mem bundle=gmem0 depth=N
    #pragma HLS INTERFACE m_axi port=output_mem bundle=gmem1 depth=N

    hls::stream<data_t> input_fifo;
    hls::stream<result_t> result_fifo;

    #pragma HLS STREAM variable=input_fifo depth=2
    #pragma HLS STREAM variable=result_fifo depth=2

    #pragma HLS DATAFLOW

    read_data(input_mem, input_fifo);
    compute(input_fifo, result_fifo);
    write_data(result_fifo, output_mem);
}