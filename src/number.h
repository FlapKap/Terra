#ifndef NUMBER_H
#define NUMBER_H

#include <stdlib.h>
#include <stdint.h>

typedef enum __attribute__((packed)) _number_union_case_t {
    NUMBER_UINT32,
    NUMBER_INT,
    NUMBER_FLOAT,
    NUMBER_DOUBLE
} number_union_case_t;

typedef struct {
    union {
        uint32_t _uint32; // case 1
        int _int;         // case 2
        float _float;     // case 3
        double _double;   // case 4
    } type;
    number_union_case_t unionCase;
} Number;


int compare(Number n1, Number n2);

#endif /* NUMBER_H */
