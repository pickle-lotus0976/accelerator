    #include <iostream>
    #include <ostream>
    #include "accelerator.h"

    int main() {
        data_t inputs[N];
        result_t outputs[N];
        result_t expected[N];

        const int weights[N] = {1, 2, 3, 4, 5, 6, 8, 9, 7, 1};

        for(int i = 0; i < N; i++) {
            inputs[i] = i;
            expected[i] = inputs[i] * weights[i];
        }

        weighted_sum_accelerator(inputs, outputs);

        int error_count = 0;
        for(int i = 0; i < N; i++) {
            if (outputs[i] != expected[i]) {
                std::cout << "Error at " << i << " :Expected " << expected[i] << " but got " << outputs[i] << std::endl;
                error_count ++;
            }
        }
        if (error_count == 0) std::cout << "Test Passed" << std::endl;
        else std::cout << "Test Failed" << std::endl;

        return error_count;
    }