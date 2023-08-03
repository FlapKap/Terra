#ifndef NUMBER_H
#define NUMBER_H

#include <stdlib.h>
#include <stdint.h>

typedef struct _Number {
    union {
        uint32_t _uint32; // case 1
        int _int;         // case 2
        float _float;     // case 3
        double _double;   // case 4
    } type;
    uint8_t unionCase;
} Number;


int compare(Number n1, Number n2);

#endif /* NUMBER_H */