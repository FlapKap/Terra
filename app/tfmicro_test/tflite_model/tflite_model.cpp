#include <stdio.h>
#include "blob/model_leak_group7_train_node_7.tflite.h"

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"

#include "tensorflow/lite/schema/schema_generated.h"

#include "tflite_model.h"

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 6 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
bool tflite_model_init() {

    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    model = tflite::GetModel(model_leak_group7_train_node_7_tflite);

    if (model->version() != TFLITE_SCHEMA_VERSION) {
        printf("Model provided is schema version %d not equal "
               "to supported version %d.",
               static_cast<uint8_t>(model->version()), TFLITE_SCHEMA_VERSION);
        return false;
    }

    // This pulls in all the operation implementations we need.
    static tflite::MicroMutableOpResolver<4> resolver;
    if (resolver.AddFullyConnected() != kTfLiteOk) {
        return false;
    }

    if (resolver.AddLogistic() != kTfLiteOk) {
        return false;
    }
    

    // Build an interpreter to run the model with.
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        puts("AllocateTensors() failed");
        return false;
    }

    // Obtain pointers to the model's input and output tensors.
    input = interpreter->input(0);
    output = interpreter->output(0);
    return true;
}

bool tflite_model_run(double* input_data, const size_t input_data_size, float* output_data, const size_t output_data_size) {
    // make sure everything is initialized
    assert(model != nullptr);
    assert(interpreter != nullptr);
    assert(input != nullptr);
    assert(output != nullptr);

    // Copy input array in input tensor
    for (unsigned i = 0; i < input_data_size; ++i) {
        input->data.f[i] = input_data[i];
    }
    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        puts("Invoke failed");
        return false;
    }

    // Copy output array from output tensor
    for (unsigned i = 0; i < output_data_size; ++i) {
        output_data[i] = output->data.f[i];
    }
    return true;
}
