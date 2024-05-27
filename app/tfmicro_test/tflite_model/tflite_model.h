#ifndef TFLITE_MODEL_H
#define TFLITE_MODEL_H

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
    bool tflite_model_init(void);
    bool tflite_model_run(double* input_data, const size_t input_data_size, float* output_data, const size_t output_data_size);
#ifdef __cplusplus
}
#endif
#endif