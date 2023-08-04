#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "operators.h"

#include "print_utils.h"
#include "debug.h"

void copy_number_to_instruction(Number *src, Instruction *dest)
{

    switch (src->unionCase)
    {
    case NUMBER_UINT32:
        dest->unionCase = INSTRUCTION_UINT32;
        dest->data._uint32 = src->type._uint32;
        break;
    case NUMBER_INT:
        dest->unionCase = INSTRUCTION_INT;
        dest->data._int = src->type._int;
        break;
    case NUMBER_FLOAT:
        dest->unionCase = INSTRUCTION_FLOAT;
        dest->data._float = src->type._float;
        break;
    case NUMBER_DOUBLE:
        dest->unionCase = INSTRUCTION_DOUBLE;
        dest->data._double = src->type._double;
        break;
    }

}

void copy_instruction_to_number(Instruction *src, Number *dest)
{
    switch (src->unionCase)
    {
    case INSTRUCTION_UINT32:
        dest->unionCase = NUMBER_UINT32;
        dest->type._uint32 = src->data._uint32;
        break;
    case INSTRUCTION_INT:
        dest->unionCase = NUMBER_INT;
        dest->type._int = src->data._int;
        break;
    case INSTRUCTION_FLOAT:
        dest->unionCase = NUMBER_FLOAT;
        dest->type._float = src->data._float;
        break;
    case INSTRUCTION_DOUBLE:
        dest->unionCase = NUMBER_DOUBLE;
        dest->type._double = src->data._double;
        break;
    default:
        DEBUG("Error: Invalid unionCase: %" PRIu8, src->unionCase);
    }

}

Number bin_op(Number n1, Number n2, ExpressionInstruction op)
{
    Number result;
    switch (op)
    {
    case ADD:
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
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
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
        case NUMBER_INT:
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int + n2.type._uint32;
                break;
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
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
            case NUMBER_INT:
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
            case NUMBER_INT:
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
    case SUB:
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

            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
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
        case NUMBER_INT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int - n2.type._uint32;
                break;
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
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
            case NUMBER_INT:
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
            case NUMBER_INT:
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
    case MUL:
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
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
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
        case NUMBER_INT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int * n2.type._uint32;
                break;
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
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
            case NUMBER_INT:
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
            case NUMBER_INT:
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
    case DIV:
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
            case NUMBER_INT:
                if (n2.type._int == 0) {
                    printf("Error: Division by zero");
                    break;
                }
                result.unionCase = NUMBER_INT;
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
        case NUMBER_INT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int / n2.type._uint32;
                break;
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
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
            case NUMBER_INT:
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
            case NUMBER_INT:
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
    case POW:
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
            case NUMBER_INT:
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
        case NUMBER_INT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._double = pow(n1.type._int, n2.type._uint32);
                break;
            case NUMBER_INT:
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
            case NUMBER_INT:
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
            case NUMBER_INT:
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
    case AND:
    {
        result.unionCase = NUMBER_INT;
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._uint32 = n1.type._uint32 && n2.type._uint32;
                break;
            case NUMBER_INT:
                result.type._int = n1.type._uint32 && n2.type._int;
                break;
            default:
                printf("Error: Invalid AND operation");
                result.type._int = 0;
            }
        }
        break;
        case NUMBER_INT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int & n2.type._uint32;
                break;
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int & n2.type._int;
                break;
            default:
                printf("Error: Invalid AND operation");
                result.type._int = 0;
                result.unionCase = NUMBER_INT;
            }
        }
        break;
        default:
            printf("Error: Invalid AND operation");
            result.type._int = 0;
            result.unionCase = NUMBER_INT;
            break;
        }
    }
    break;
    case OR:
    {
        result.unionCase = NUMBER_INT;
        switch (n1.unionCase)
        {
        case NUMBER_UINT32:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._uint32 = n1.type._uint32 || n2.type._uint32;
                break;
            case NUMBER_INT:
                result.type._int = n1.type._uint32 || n2.type._int;
                break;
            default:
                printf("Error: Invalid OR operation");
                result.type._int = 0;
                break;
            }
        }
        break;
        case NUMBER_INT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.type._int = n1.type._int || n2.type._uint32;
                break;
            case NUMBER_INT:
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
    case LT:
    {
        result.unionCase = NUMBER_INT;
        result.type._int = compare(n1, n2) == -1;

        break;
    }
    case GT:
    {
        result.unionCase = NUMBER_INT;
        result.type._int = compare(n1, n2) == 1;
        break;
    }
    case EQ:
    {
        result.unionCase = NUMBER_INT;
        result.type._int = compare(n1, n2) == 0;
        break;
    }
    case LTEQ:
    {
        result.unionCase = NUMBER_INT;
        result.type._int = compare(n1, n2) <= 0;
        break;
    }
    case GTEQ:
    {
        result.unionCase = NUMBER_INT;
        result.type._int = compare(n1, n2) >= 0;
        break;
    }
    case MOD:
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
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._uint32 % n2.type._int;
                break;

            default:
                printf("Error: Invalid MOD operation. Divisor must be integer");
                result.type._int = 0;
                result.unionCase = NUMBER_INT;
                break;
            }
            break;
        }
        case NUMBER_INT:
        {
            switch (n2.unionCase)
            {
            case NUMBER_UINT32:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int % n2.type._uint32;
                break;
            case NUMBER_INT:
                result.unionCase = NUMBER_INT;
                result.type._int = n1.type._int % n2.type._int;
                break;
            default:
                printf("Error: Invalid MOD operation. Divisor must be integer");
                result.type._int = 0;
                result.unionCase = NUMBER_INT;
                break;
            }
        }
        break;
        default:
            printf("Error: Invalid MOD operation. Dividend must be integer");
            result.type._int = 0;
            result.unionCase = NUMBER_INT;
            break;
        }
    }
    break;
    case VAR:
    case CONST:
    case NOT:
    case LOG:
    case SQRT:
    case EXP:
    case CEIL:
    case FLOOR:
    case ROUND:
    case ABS:
        printf("Error: Unary operation in bin_op");
        break;
    }
    return result;
}

Number un_op(Number number, ExpressionInstruction op)
{
    Number result;
    double value = 0.0;

    // Extract the value from the Number union
    switch (number.unionCase)
    {
    case NUMBER_UINT32:
        value = (double)number.type._uint32;
        break;
    case NUMBER_INT:
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
    case SQRT:
        result.type._double = sqrt(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    case EXP:
        result.type._double = exp(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    case CEIL:
        result.type._int = (int)ceil(value);
        result.unionCase = NUMBER_INT;
        break;
    case FLOOR:
        result.type._int = (int)floor(value);
        result.unionCase = NUMBER_INT;
        break;
    case ROUND:
        result.type._int =  (int) round(value);
        result.unionCase = NUMBER_INT;
        break;
    case ABS:
        result.type._double = fabs(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    case NOT:
        result.type._int = !(int)value;
        result.unionCase = NUMBER_INT;
        break;
    case LOG:
        result.type._double = log(value);
        result.unionCase = NUMBER_DOUBLE;
        break;
    default:
        printf("Error: Invalid unary operation");
    }

    // Return the result
    return result;
}