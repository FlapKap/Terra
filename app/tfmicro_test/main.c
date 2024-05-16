#include <stdio.h>
#include "model_leak_group7_train_node_7.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

int main(void)
{
    puts("Hello World! tflite-micro test");
    puts(__TIME__);
    printf("length: %ud\n", model_leak_group7_train_node_7_tflite_len);


    return 0;
}