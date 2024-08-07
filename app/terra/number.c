
#include <math.h>
#include "number.h"

#include "debug.h"

static int compare_uint32(uint32_t n1, uint32_t n2)
{
    if (n1 > n2)
    {
        return 1;
    }
    else if (n1 < n2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int compare_int(int n1, int n2)
{
    if (n1 > n2)
    {
        return 1;
    }
    else if (n1 < n2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
static int compare_float(float n1, float n2)
{
    if (n1 > n2)
    {
        return 1;
    }
    else if (n1 < n2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int compare_double(double n1, double n2)
{
    if (n1 > n2)
    {
        return 1;
    }
    else if (n1 < n2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int compare_uint32_with_int(uint32_t n1, int n2)
{
    return compare_int((int)n1, n2);
}
static int compare_int_with_uint32(int n1, uint32_t n2)
{
    return compare_int(n1, (int)n2);
}

static int compare_int_with_float(int n1, float n2)
{
    return compare_float((float)n1, n2);
}
static int compare_float_with_int(float n1, int n2)
{
    return compare_float(n1, (float)n2);
}

static int compare_int_with_double(int n1, double n2)
{
    return compare_double((double)n1, n2);
}
static int compare_double_with_int(double n1, int n2)
{
    return compare_double(n1, (double)n2);
}

static int compare_float_with_uint32(float n1, uint32_t n2)
{
    return compare_float(n1, (float)n2);
}
static int compare_uint32_with_float(uint32_t n1, float n2)
{
    return compare_float((float)n1, n2);
}

static int compare_float_with_double(float n1, double n2)
{
    return compare_double((double)n1, n2);
}
static int compare_double_with_float(double n1, float n2)
{
    return compare_double(n1, (double)n2);
}

static int compare_double_with_uint32(double n1, uint32_t n2)
{
    return compare_double(n1, (double)n2);
}
static int compare_uint32_with_double(uint32_t n1, double n2)
{
    return compare_double((double)n1, n2);
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
int number_compare(const Number *n1, const Number *n2)
{
    switch (n1->unionCase)
    {
    case NUMBER_UINT32:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            return compare_uint32(n1->type._uint32, n2->type._uint32);
        case NUMBER_INT32:
            return compare_uint32_with_int(n1->type._uint32, n2->type._int);
        case NUMBER_FLOAT:
            return compare_uint32_with_float(n1->type._uint32, n2->type._float);
        case NUMBER_DOUBLE:
            return compare_uint32_with_double(n1->type._uint32, n2->type._double);
        }
        break;
    case NUMBER_INT32:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            return compare_int_with_uint32(n1->type._int, n2->type._uint32);
        case NUMBER_INT32:
            return compare_int(n1->type._int, n2->type._int);
        case NUMBER_FLOAT:
            return compare_int_with_float(n1->type._int, n2->type._float);
        case NUMBER_DOUBLE:
            return compare_int_with_double(n1->type._int, n2->type._double);
        }
        break;
    case NUMBER_FLOAT:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            return compare_float_with_uint32(n1->type._float, n2->type._uint32);
        case NUMBER_INT32:
            return compare_float_with_int(n1->type._float, n2->type._int);
        case NUMBER_FLOAT:
            return compare_float(n1->type._float, n2->type._float);
        case NUMBER_DOUBLE:
            return compare_float_with_double(n1->type._float, n2->type._double);
        }
        break;
    case NUMBER_DOUBLE:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            return compare_double_with_uint32(n1->type._double, n2->type._uint32);
        case NUMBER_INT32:
            return compare_double_with_int(n1->type._double, n2->type._int);
        case NUMBER_FLOAT:
            return compare_double_with_float(n1->type._double, n2->type._float);
        case NUMBER_DOUBLE:
            return compare_double(n1->type._double, n2->type._double);
        }
        break;
    }
    return 0;
}

void number_negate(const Number *n, Number *result)
{
    result->unionCase = n->unionCase;
    switch (n->unionCase)
    {
    case NUMBER_UINT32:
        result->type._int = -((int)n->type._uint32);
        result->unionCase = NUMBER_INT32;
        break;
    case NUMBER_INT32:
        result->type._int = -n->type._int;
        break;
    case NUMBER_FLOAT:
        result->type._float = -n->type._float;
        break;
    case NUMBER_DOUBLE:
        result->type._double = -n->type._double;
        break;
    }
}

void number_add(const Number *n1, const Number *n2, Number *result)
{
    switch (n1->unionCase)
    {
    case NUMBER_UINT32:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._uint32 = n1->type._uint32 + n2->type._uint32;
            result->unionCase = NUMBER_UINT32;
            break;
        case NUMBER_INT32:
            result->type._int = n1->type._uint32 + n2->type._int;
            result->unionCase = NUMBER_INT32;
            break;
        case NUMBER_FLOAT:
            result->type._double = ((double)n1->type._uint32) + n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = ((double)n1->type._uint32) + n2->type._double;
            break;
        }
        break;
    case NUMBER_INT32:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._int = n1->type._int + n2->type._uint32;
            result->unionCase = NUMBER_INT32;
            break;
        case NUMBER_INT32:
            result->type._int = n1->type._int + n2->type._int;
            result->unionCase = NUMBER_INT32;
            break;
        case NUMBER_FLOAT:
            result->type._double = ((double)n1->type._int) + n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = ((double)n1->type._int) + n2->type._double;
            break;
        }
        break;
    case NUMBER_FLOAT:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._double = n1->type._float + ((double)n2->type._uint32);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_INT32:
            result->type._double = n1->type._float + ((double)n2->type._int);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_FLOAT:
            result->type._double = n1->type._float + n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = n1->type._float + n2->type._double;
            break;
        }
        break;
    case NUMBER_DOUBLE:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._double = n1->type._double + ((double)n2->type._uint32);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_INT32:
            result->type._double = n1->type._double + ((double)n2->type._int);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_FLOAT:
            result->type._double = n1->type._double + n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = n1->type._double + n2->type._double;
            break;
        }
        break;
    }
}

void number_sub(const Number *n1, const Number *n2, Number *result)
{
    Number temp;
    number_negate(n1, &temp);
    number_add(&temp, n2, result);
}

void number_div(const Number *n1, const Number *n2, Number *result)
{
    switch (n1->unionCase)
    {
    case NUMBER_UINT32:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._uint32 = n1->type._uint32 / n2->type._uint32;
            result->unionCase = NUMBER_UINT32;
            break;
        case NUMBER_INT32:
            result->type._uint32 = n1->type._uint32 / n2->type._int;
            result->unionCase = NUMBER_UINT32;
            break;
        case NUMBER_FLOAT:
            result->type._double = ((double)n1->type._uint32) / n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = ((double)n1->type._uint32) / n2->type._double;
            break;
        }
        break;
    case NUMBER_INT32:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._int = n1->type._int / n2->type._uint32;
            result->unionCase = NUMBER_INT32;
            break;
        case NUMBER_INT32:
            result->type._int = n1->type._int / n2->type._int;
            result->unionCase = NUMBER_INT32;
            break;
        case NUMBER_FLOAT:
            result->type._double = ((double)n1->type._int) / n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = ((double)n1->type._int) / n2->type._double;
            break;
        }
        break;
    case NUMBER_FLOAT:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._double = n1->type._float / ((double)n2->type._uint32);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_INT32:
            result->type._double = n1->type._float / ((double)n2->type._int);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_FLOAT:
            result->type._double = n1->type._float / n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = n1->type._float / n2->type._double;
            break;
        }
        break;
    case NUMBER_DOUBLE:
        switch (n2->unionCase)
        {
        case NUMBER_UINT32:
            result->type._double = n1->type._double / ((double)n2->type._uint32);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_INT32:
            result->type._double = n1->type._double / ((double)n2->type._int);
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_FLOAT:
            result->type._double = n1->type._double / n2->type._float;
            result->unionCase = NUMBER_DOUBLE;
            break;
        case NUMBER_DOUBLE:
            result->type._double = n1->type._double / n2->type._double;
            break;
        }
        break;
    }
}


bool number_is_false(const Number *n)
{
    return n->type._int == 0 && (n->unionCase == NUMBER_UINT32 || n->unionCase == NUMBER_INT32);
}
