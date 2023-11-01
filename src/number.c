
#include <math.h>
#include "number.h"

#include "debug.h"

static int compare_uint32(uint32_t n1, uint32_t n2){
    if(n1 > n2){
        return 1;
    } else if (n1 < n2){
        return -1;
    } else {
        return 0;
    }
}

static int compare_int(int n1, int n2){
    if(n1 > n2){
        return 1;
    } else if (n1 < n2){
        return -1;
    } else {
        return 0;
    }
}
static int compare_float(float n1, float n2) {
    if (n1 > n2) {
        return 1;
    } else if (n1 < n2) {
        return -1;
    } else {
        return 0;
    }
}

static int compare_double(double n1, double n2) {
    if (n1 > n2) {
        return 1;
    } else if (n1 < n2) {
        return -1;
    } else {
        return 0;
    }
}

static int compare_uint32_with_int(uint32_t n1, int n2) {
    return compare_int((int)n1, n2);
}
static int compare_int_with_uint32(int n1, uint32_t n2) {
    return compare_int(n1, (int)n2);
}

static int compare_int_with_float(int n1, float n2) {
    return compare_float((float)n1, n2);
}
static int compare_float_with_int(float n1, int n2) {
    return compare_float(n1, (float)n2);
}

static int compare_int_with_double(int n1, double n2) {
    return compare_double((double)n1, n2);
}
static int compare_double_with_int(double n1, int n2) {
    return compare_double(n1, (double)n2);
}

static int compare_float_with_uint32(float n1, uint32_t n2) {
    return compare_float(n1, (float)n2);
}
static int compare_uint32_with_float(uint32_t n1, float n2) {
    return compare_float((float) n1, n2);
}

static int compare_float_with_double(float n1, double n2) {
    return compare_double((double)n1, n2);
}
static int compare_double_with_float(double n1, float n2) {
    return compare_double(n1, (double)n2);
}

static int compare_double_with_uint32(double n1, uint32_t n2) {
    return compare_double(n1, (double)n2);
}
static int compare_uint32_with_double(uint32_t n1, double n2) {
    return compare_double((double) n1, n2);
}


/*
* Compare two numbers.
*
* Args:
*     n1 (Number): The first number.
*     n2 (Number): The second number.
*
* Returns:
*     int: -1 if n1 < n2, 0 if n1 == n2, 1 if n1 > n2.
*/
int compare(Number n1, Number n2){
    switch(n1.unionCase){
        case NUMBER_UINT32:
            switch(n2.unionCase){
                case NUMBER_UINT32:
                    return compare_uint32(n1.type._uint32, n2.type._uint32);
                case NUMBER_INT:
                    return compare_uint32_with_int(n1.type._uint32, n2.type._int);
                case NUMBER_FLOAT:
                    return compare_uint32_with_float(n1.type._uint32, n2.type._float);
                case NUMBER_DOUBLE:
                    return compare_uint32_with_double(n1.type._uint32, n2.type._double);
            }
            break;
        case NUMBER_INT:
            switch(n2.unionCase){
                case NUMBER_UINT32:
                    return compare_int_with_uint32(n1.type._int, n2.type._uint32);
                case NUMBER_INT:
                    return compare_int(n1.type._int, n2.type._int);
                case NUMBER_FLOAT:
                    return compare_int_with_float(n1.type._int, n2.type._float);
                case NUMBER_DOUBLE:
                    return compare_int_with_double(n1.type._int, n2.type._double);
            }
            break;
        case NUMBER_FLOAT:
            switch(n2.unionCase){
                case NUMBER_UINT32:
                    return compare_float_with_uint32(n1.type._float, n2.type._uint32);
                case NUMBER_INT:
                    return compare_float_with_int(n1.type._float, n2.type._int);
                case NUMBER_FLOAT:
                    return compare_float(n1.type._float, n2.type._float);
                case NUMBER_DOUBLE:
                    return compare_float_with_double(n1.type._float, n2.type._double);
            }
            break;
        case NUMBER_DOUBLE:
            switch(n2.unionCase){
                case NUMBER_UINT32:
                    return compare_double_with_uint32(n1.type._double, n2.type._uint32);
                case NUMBER_INT:
                    return compare_double_with_int(n1.type._double, n2.type._int);
                case NUMBER_FLOAT:
                    return compare_double_with_float(n1.type._double, n2.type._float);
                case NUMBER_DOUBLE:
                    return compare_double(n1.type._double, n2.type._double);
            }
            break;
    }
    return 0;
}
