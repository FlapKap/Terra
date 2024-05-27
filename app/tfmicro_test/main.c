#include <stdio.h>
#include <inttypes.h>
#include "tflite_model/tflite_model.h"
#include "tflite_model/data.h"
int main(void)
{
    printf("Hello, World!\n");

    tflite_model_init();
    float output[1] = { 0 };
    for(size_t i = 0; i < DATA_LEN; i++) {
        tflite_model_run(data[i], 38, output, 1);
        printf("Output: %f\n", output[0]);
    }

    //print output
    printf("done!\n");

    return 0;
}