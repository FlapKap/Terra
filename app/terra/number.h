#ifndef NUMBER_H
#define NUMBER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum __attribute__((packed)) _number_union_case_t {
    NUMBER_UINT32,
    NUMBER_INT32,
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


int number_compare(const Number* n1, const Number* n2);
void number_negate(const Number* n, Number* result);
void number_add(const Number* n1, const Number* n2, Number* result);
void number_sub(const Number* n1, const Number* n2, Number* result);
void number_div(const Number* n1, const Number* n2, Number* result);
bool number_is_false(const Number*);

#endif /* NUMBER_H */
