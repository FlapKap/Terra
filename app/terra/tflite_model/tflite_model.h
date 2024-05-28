#ifndef TFLITE_MODEL_H
#define TFLITE_MODEL_H

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include "../number.h"

#ifdef __cplusplus
extern "C" {
#endif
    /** Initialize the model 
     * @returns true if successful
    */
    bool tflite_model_init(void);

    /** Run the model
     * @param input_data pointer to the input data.
     * @param input_data_size the size of the input data
     * @param output_data the output data
     * @param output_data_size the size of the output data
     * @returns true if successful
     * 
     * @note Handling of the input and output data must currently be defined in tflite_model.cpp in the functions tflite_model_copy_input and tflite_model_copy_output
    */
    bool tflite_model_run(const Number* input_data, const size_t input_data_size, Number* output_data, const size_t output_data_size);
#ifdef __cplusplus
}
#endif
#endif /* TFLITE_MODEL_H */