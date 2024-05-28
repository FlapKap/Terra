#include <stdio.h>
#include "blob/model_leak_group7_train_node_7.tflite.h"

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"

#include "tensorflow/lite/schema/schema_generated.h"

#include "tflite_model.h"

#define ENABLE_DEBUG 1
#include <debug.h>

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

//DEFINE INPUT AND OUTPUT FUNCTIONS HERE
//the below 2 functions copy the input data into the input tensor and the output data from the output tensor
//since these are dependent on the type of the model they need to be defined per model
static bool tflite_model_copy_input(const Number* input_data, const size_t input_data_size) {
    // Copy input array in input tensor

    //print data to debug
    double test_data[] = {-0.78670334, -111104808, -0.75984997,-0.78614863, 0.61591590, 0.39533073,-0.77594615, -0.99062349, -0.76685757, -0.76339546, -0.91252439, -0.81700908, -0.80688303, -0.39523543, -0.63286853, -0.68540174, -0.42793395, -0.72349461, -0.73508538, -110545106, -0.80093000, -0.79739966, -105830462, -0.77096873, -0.79826535, -105136734, -0.70872860, -0.77018562, -105119534, -0.77705343, -0.76926486, -0.38729043, -0.65491056, -0.64664830, -0.91299976, -0.66099265, -0.64731443, -0.60991834};
    for (unsigned i = 0; i < ARRAY_SIZE(test_data); ++i) {
        DEBUG("input_data[%d] = %f\n", i, test_data[i]);
        input->data.f[i] = test_data[i];
    }
    return true;
}

static bool tflite_model_copy_output(Number* output_data, const size_t output_data_size) {
    // Copy output array from output tensor
    for (unsigned i = 0; i < output_data_size; ++i) {
        DEBUG("output_data[%d] = %f\n", i, output->data.f[i]);
        output_data[i].type._double = output->data.f[i];
    }
    return true;
}



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

    // DEFINE OPERATIONS HERE:
    // This pulls in all the operation implementations we need.
    static tflite::MicroMutableOpResolver<2> resolver;
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

    DEBUG("tflite initialized\n");
    DEBUG("\tinput dims: [");
    for (int i = 0; i < input->dims->size; ++i) {
        DEBUG("%d, ", input->dims->data[i]);
    }
    DEBUG("]\n");
    DEBUG("\toutput dims: [");
    for (int i = 0; i < output->dims->size; ++i) {
        DEBUG("%d, ", output->dims->data[i]);
    }
    DEBUG("]\n");
    DEBUG("\tarena size: %d\n", interpreter->arena_used_bytes());


    return true;
}



bool tflite_model_run(const Number* input_data, const size_t input_data_size, Number* output_data, const size_t output_data_size) {
    // make sure everything is initialized
    assert(model != nullptr);
    assert(interpreter != nullptr);
    assert(input != nullptr);
    assert(output != nullptr);

    // Copy input array in input tensor
    tflite_model_copy_input(input_data, input_data_size);
    // Run inference, and report any error
    DEBUG("TFLITE: running inference\n");
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        DEBUG("Invoke failed");
        return false;
    }

    // Copy output array from output tensor
    tflite_model_copy_output(output_data, output_data_size);
    return true;
}

