#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "operators.h"

#include "print_utils.h"
#include "debug.h"

void copy_number_to_instruction(Number *src, TerraProtocol_Data *dest)
{

    switch (src->unionCase)
    {
    case NUMBER_UINT32:
        dest->which_data = TerraProtocol_Data__uint32_tag;
        dest->data._uint32 = src->type._uint32;
        break;
    case NUMBER_INT32:
        dest->which_data = TerraProtocol_Data__int32_tag;
        dest->data._int32 = src->type._int;
        break;
    case NUMBER_FLOAT:
        dest->which_data = TerraProtocol_Data__float_tag;
        dest->data._float = src->type._float;
        break;
    case NUMBER_DOUBLE:
        dest->which_data = TerraProtocol_Data__double_tag;
        dest->data._double = src->type._double;
        break;
    }

}

void copy_instruction_to_number(TerraProtocol_Data *src, Number *dest)
{
    switch (src->which_data)
    {
        //these case-fallthroughs relies on union-cases always
        // expanding to the biggest type.
        // I.e. we can still read a uint8 as if it where a uint32
        case TerraProtocol_Data__uint8_tag:
        case TerraProtocol_Data__uint16_tag:
        case TerraProtocol_Data__uint32_tag:
            dest->unionCase = NUMBER_UINT32;
            dest->type._uint32 = src->data._uint32;
            break;
        case TerraProtocol_Data__int8_tag:
        case TerraProtocol_Data__int16_tag:
        case TerraProtocol_Data__int32_tag:
            dest->unionCase = NUMBER_INT32;
            dest->type._int = src->data._int32;
            break;
        case TerraProtocol_Data__float_tag:
            dest->unionCase = NUMBER_FLOAT;
            dest->type._float = src->data._float;
            break;
        case TerraProtocol_Data__double_tag:
            dest->unionCase = NUMBER_DOUBLE;
            dest->type._double = src->data._double;
            break;
    default:
        DEBUG("Error: Invalid unionCase: %" PRIu8, src->which_data);
    }

}

Number bin_op(Number n1, Number n2, TerraProtocol_ExpressionInstructions op)
{
    Number result = { 0 };
    switch (op)
    {
    case TerraProtocol_ADD:
    {
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_UINT32;
                result.type._uint32 = n1.type._uint32 + n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._uint32 + n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._uint32 + n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._uint32 + n2.type._double;
                break;
            }
            break;
        case NUMBER_INT32:
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int + n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int + n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._int + n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._int + n2.type._double;
                break;
            }
            break;
        case NUMBER_FLOAT:
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float + n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float + n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float + n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._float + n2.type._double;
                break;
            }
            break;
        case NUMBER_DOUBLE:
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double + n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double + n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double + n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double + n2.type._double;
                break;
            }
            break;
        }
    }
    break;
    case TerraProtocol_SUB:
    {
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_UINT32;
                result.type._uint32 = n1.type._uint32 - n2.type._uint32;
                break;

            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._uint32 - n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._uint32 - n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._uint32 - n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_INT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int - n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int - n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._int - n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._int - n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_FLOAT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float - n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float - n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float - n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._float - n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_DOUBLE:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double - n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double - n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double - n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double - n2.type._double;
                break;
            }
        }
        break;
        }
    }
    break;
    case TerraProtocol_MUL:
    {
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_UINT32;
                result.type._uint32 = n1.type._uint32 * n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._uint32 * n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._uint32 * n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._uint32 * n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_INT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int * n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int * n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._int * n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._int * n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_FLOAT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float * n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float * n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float * n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._float * n2.type._double;
            }
        }
        break;
        case NUMBER_DOUBLE:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double * n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double * n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double * n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double * n2.type._double;
                break;
            }
        }
        break;
        }
        break;
    }
    break;
    case TerraProtocol_DIV:
    {
        switch (n1.unionCase)
        {
        case NUMBER_UINT32: // TODO: fix division by zero with fpclassify
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                if (n2.type._uint32 == 0){
                    printf("Error: Division by zero");
                    break;
                }

                result.unionCase = NUMBER_UINT32;
                result.type._uint32 = n1.type._uint32 / n2.type._uint32;
                break;
            case NUMBER_INT32:
                if (n2.type._int == 0) {
                    printf("Error: Division by zero");
                    break;
                }
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._uint32 / n2.type._int;
                break;
            case NUMBER_FLOAT:
                if (n2.type._float == 0.0f){
                    printf("Error: Division by zero");
                    break;
                }
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._uint32 / n2.type._float;
                break;
            case NUMBER_DOUBLE:
                if (n2.type._double == 0.0){
                    printf("Error: Division by zero");
                    break;
                }
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._uint32 / n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_INT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int / n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int / n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._int / n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._int / n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_FLOAT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float / n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float / n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_FLOAT;
                result.type._float = n1.type._float / n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._float / n2.type._double;
                break;
            }
        }
        break;
        case NUMBER_DOUBLE:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double / n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double / n2.type._int;
                break;
            case NUMBER_FLOAT:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double / n2.type._float;
                break;
            case NUMBER_DOUBLE:
                result.unionCase = NUMBER_DOUBLE;
                result.type._double = n1.type._double / n2.type._double;
                break;
            }
        }
        break;
        }
    }
    break;
    case TerraProtocol_POW:
    {
        result.unionCase = NUMBER_DOUBLE;
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._double = pow(n1.type._uint32, n2.type._uint32);
                break;
            case NUMBER_INT32:
                result.type._double = pow(n1.type._uint32, n2.type._int);
                break;
            case NUMBER_FLOAT:
                result.type._double = pow(n1.type._uint32, n2.type._float);
                break;
            case NUMBER_DOUBLE:
                result.type._double = pow(n1.type._uint32, n2.type._double);
                break;
            }
        }
        break;
        case NUMBER_INT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._double = pow(n1.type._int, n2.type._uint32);
                break;
            case NUMBER_INT32:
                result.type._double = pow(n1.type._int, n2.type._int);
                break;
            case NUMBER_FLOAT:
                result.type._double = pow(n1.type._int, n2.type._float);
                break;
            case NUMBER_DOUBLE:
                result.type._double = pow(n1.type._int, n2.type._double);
                break;
            }
        }
        break;
        case NUMBER_FLOAT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._double = pow(n1.type._float, n2.type._uint32);
                break;
            case NUMBER_INT32:
                result.type._double = pow(n1.type._float, n2.type._int);
                break;
            case NUMBER_FLOAT:
                result.type._double = pow(n1.type._float, n2.type._float);
                break;
            case NUMBER_DOUBLE:
                result.type._double = pow(n1.type._float, n2.type._double);
                break;
            }
        }
        break;
        case NUMBER_DOUBLE:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._double = pow(n1.type._double, n2.type._uint32);
                break;
            case NUMBER_INT32:
                result.type._double = pow(n1.type._double, n2.type._int);
                break;
            case NUMBER_FLOAT:
                result.type._double = pow(n1.type._double, n2.type._float);
                break;
            case NUMBER_DOUBLE:
                result.type._double = pow(n1.type._double, n2.type._double);
                break;
            }
        }
        break;
        }
    }
    break;
    case TerraProtocol_AND:
    {
        result.unionCase = NUMBER_INT32;
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._uint32 = n1.type._uint32 && n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.type._int = n1.type._uint32 && n2.type._int;
                break;
            default:
                printf("Error: Invalid AND operation");
                result.type._int = 0;
            }
        }
        break;
        case NUMBER_INT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int & n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int & n2.type._int;
                break;
            default:
                printf("Error: Invalid AND operation");
                result.type._int = 0;
                result.unionCase = NUMBER_INT32;
            }
        }
        break;
        default:
            printf("Error: Invalid AND operation");
            result.type._int = 0;
            result.unionCase = NUMBER_INT32;
            break;
        }
    }
    break;
    case TerraProtocol_OR:
    {
        result.unionCase = NUMBER_INT32;
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._uint32 = n1.type._uint32 || n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.type._int = n1.type._uint32 || n2.type._int;
                break;
            default:
                printf("Error: Invalid OR operation");
                result.type._int = 0;
                break;
            }
        }
        break;
        case NUMBER_INT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._int = n1.type._int || n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.type._int = n1.type._int || n2.type._int;
                break;
            default:
                printf("Error: Invalid OR operation");
                result.type._int = 0;
                break;
            }
        }
        break;
        default:
            printf("Error: Invalid OR operation");
            result.type._int = 0;
            break;
        }
    }
    break;
    case TerraProtocol_LT:
    {
        result.unionCase = NUMBER_INT32;
        result.type._int = compare(&n1, &n2) == -1;

        break;
    }
    case TerraProtocol_GT:
    {
        result.unionCase = NUMBER_INT32;
        result.type._int = compare(&n1, &n2) == 1;
        break;
    }
    case TerraProtocol_EQ:
    {
        result.unionCase = NUMBER_INT32;
        result.type._int = compare(&n1, &n2) == 0;
        break;
    }
    case TerraProtocol_LTEQ:
    {
        result.unionCase = NUMBER_INT32;
        result.type._int = compare(&n1, &n2) <= 0;
        break;
    }
    case TerraProtocol_GTEQ:
    {
        result.unionCase = NUMBER_INT32;
        result.type._int = compare(&n1, &n2) >= 0;
        break;
    }
    case TerraProtocol_MOD:
    {
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_UINT32;
                result.type._uint32 = n1.type._uint32 % n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._uint32 % n2.type._int;
                break;

            default:
                printf("Error: Invalid MOD operation. Divisor must be integer");
                result.type._int = 0;
                result.unionCase = NUMBER_INT32;
                break;
            }
            break;
        }
        case NUMBER_INT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int % n2.type._uint32;
                break;
            case NUMBER_INT32:
                result.unionCase = NUMBER_INT32;
                result.type._int = n1.type._int % n2.type._int;
                break;
            default:
                printf("Error: Invalid MOD operation. Divisor must be integer");
                result.type._int = 0;
                result.unionCase = NUMBER_INT32;
                break;
            }
        }
        break;
        default:
            printf("Error: Invalid MOD operation. Dividend must be integer");
            result.type._int = 0;
            result.unionCase = NUMBER_INT32;
            break;
        }
    }
    break;
    case TerraProtocol_VAR:
    case TerraProtocol_CONST:
    case TerraProtocol_NOT:
    case TerraProtocol_LOG:
    case TerraProtocol_SQRT:
    case TerraProtocol_EXP:
    case TerraProtocol_CEIL:
    case TerraProtocol_FLOOR:
    case TerraProtocol_ROUND:
    case TerraProtocol_ABS:
        DEBUG("Error: Unary operation in bin_op: %s", TerraProtocol_ExpressionInstructions_name(op));
        break;
    }
    return result;
}

Number un_op(Number number, TerraProtocol_ExpressionInstructions op)
{
    Number result = { 0 };
    double value = 0.0;

    // Extract the value from the Number union
    switch (number.unionCase)
    {
    case NUMBER_UINT32:
        value = (double)number.type._uint32;
        break;
    case NUMBER_INT32:
        value = (double)number.type._int;
        break;
    case NUMBER_FLOAT:
        value = (double)number.type._float;
        break;
    case NUMBER_DOUBLE:
        value = number.type._double;
        break;
    }

    // Perform the operation based on the given instruction
    switch (op)
    {
    case TerraProtocol_SQRT:
        result.type._double = sqrt(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    case TerraProtocol_EXP:
        result.type._double = exp(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    case TerraProtocol_CEIL:
        result.type._int = (int)ceil(value);
        result.unionCase = NUMBER_INT32;
        break;
    case TerraProtocol_FLOOR:
        result.type._int = (int)floor(value);
        result.unionCase = NUMBER_INT32;
        break;
    case TerraProtocol_ROUND:
        result.type._int =  (int) round(value);
        result.unionCase = NUMBER_INT32;
        break;
    case TerraProtocol_ABS:
        result.type._double = fabs(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    case TerraProtocol_NOT:
        result.type._int = !(int)value;
        result.unionCase = NUMBER_INT32;
        break;
    case TerraProtocol_LOG:
        result.type._double = log(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    default:
        printf("Error: Invalid unary operation: %s", TerraProtocol_ExpressionInstructions_name(op));
    }

    // Return the result
    return result;
}
