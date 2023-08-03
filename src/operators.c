#include <stdio.h>
#include <math.h>
#include "operators.h"

Number bin_op(Number n1, Number n2, ExpressionInstruction op)
{
    Number result;
    switch (op)
    {
    case ADD:
    {
        switch (n1.unionCase)
        {
        case 1:
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 + n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._uint32 + n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._uint32 + n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._uint32 + n2.type._double;
                break;
            }
            break;
        case 2:
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 2;
                result.type._int = n1.type._int + n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._int + n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._int + n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._int + n2.type._double;
                break;
            }
            break;
        case 3:
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 3;
                result.type._float = n1.type._float + n2.type._uint32;
                break;
            case 2:
                result.unionCase = 3;
                result.type._float = n1.type._float + n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._float + n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._float + n2.type._double;
                break;
            }
            break;
        case 4:
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 4;
                result.type._double = n1.type._double + n2.type._uint32;
                break;
            case 2:
                result.unionCase = 4;
                result.type._double = n1.type._double + n2.type._int;
                break;
            case 3:
                result.unionCase = 4;
                result.type._double = n1.type._double + n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
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
        case 1:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 - n2.type._uint32;
                break;

            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._uint32 - n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._uint32 - n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._uint32 - n2.type._double;
                break;
            }
        }
        break;
        case 2:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 2;
                result.type._int = n1.type._int - n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._int - n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._int - n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._int - n2.type._double;
                break;
            }
        }
        break;
        case 3:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 3;
                result.type._float = n1.type._float - n2.type._uint32;
                break;
            case 2:
                result.unionCase = 3;
                result.type._float = n1.type._float - n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._float - n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._float - n2.type._double;
                break;
            }
        }
        break;
        case 4:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 4;
                result.type._double = n1.type._double - n2.type._uint32;
                break;
            case 2:
                result.unionCase = 4;
                result.type._double = n1.type._double - n2.type._int;
                break;
            case 3:
                result.unionCase = 4;
                result.type._double = n1.type._double - n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
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
        case 1:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 * n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._uint32 * n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._uint32 * n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._uint32 * n2.type._double;
                break;
            }
        }
        break;
        case 2:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 2;
                result.type._int = n1.type._int * n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._int * n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._int * n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._int * n2.type._double;
                break;
            }
        }
        break;
        case 3:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 3;
                result.type._float = n1.type._float * n2.type._uint32;
                break;
            case 2:
                result.unionCase = 3;
                result.type._float = n1.type._float * n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._float * n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._float * n2.type._double;
            }
        }
        break;
        case 4:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 4;
                result.type._double = n1.type._double * n2.type._uint32;
                break;
            case 2:
                result.unionCase = 4;
                result.type._double = n1.type._double * n2.type._int;
                break;
            case 3:
                result.unionCase = 4;
                result.type._double = n1.type._double * n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
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
        case 1: // TODO: fix division by zero with fpclassify
        {
            switch (n2.unionCase)
            {
            case 1:
                if (n2.type._uint32 == 0)
                    printf("Error: Division by zero");
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 / n2.type._uint32;
                break;
            case 2:
                if (n2.type._int == 0)
                    printf("Error: Division by zero");
                result.unionCase = 2;
                result.type._int = n1.type._uint32 / n2.type._int;
                break;
            case 3:
                if (n2.type._float == 0.0f)
                    printf("Error: Division by zero");
                result.unionCase = 3;
                result.type._float = n1.type._uint32 / n2.type._float;
                break;
            case 4:
                if (n2.type._double == 0.0)
                    printf("Error: Division by zero");
                result.unionCase = 4;
                result.type._double = n1.type._uint32 / n2.type._double;
                break;
            }
        }
        break;
        case 2:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 2;
                result.type._int = n1.type._int / n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._int / n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._int / n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._int / n2.type._double;
                break;
            }
        }
        break;
        case 3:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 3;
                result.type._float = n1.type._float / n2.type._uint32;
                break;
            case 2:
                result.unionCase = 3;
                result.type._float = n1.type._float / n2.type._int;
                break;
            case 3:
                result.unionCase = 3;
                result.type._float = n1.type._float / n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
                result.type._double = n1.type._float / n2.type._double;
                break;
            }
        }
        break;
        case 4:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 4;
                result.type._double = n1.type._double / n2.type._uint32;
                break;
            case 2:
                result.unionCase = 4;
                result.type._double = n1.type._double / n2.type._int;
                break;
            case 3:
                result.unionCase = 4;
                result.type._double = n1.type._double / n2.type._float;
                break;
            case 4:
                result.unionCase = 4;
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
        result.unionCase = 4;
        switch (n1.unionCase)
        {
        case 1:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.type._double = pow(n1.type._uint32, n2.type._uint32);
                break;
            case 2:
                result.type._double = pow(n1.type._uint32, n2.type._int);
                break;
            case 3:
                result.type._double = pow(n1.type._uint32, n2.type._float);
                break;
            case 4:
                result.type._double = pow(n1.type._uint32, n2.type._double);
                break;
            }
        }
        break;
        case 2:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.type._double = pow(n1.type._int, n2.type._uint32);
                break;
            case 2:
                result.type._double = pow(n1.type._int, n2.type._int);
                break;
            case 3:
                result.type._double = pow(n1.type._int, n2.type._float);
                break;
            case 4:
                result.type._double = pow(n1.type._int, n2.type._double);
                break;
            }
        }
        break;
        case 3:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.type._double = pow(n1.type._float, n2.type._uint32);
                break;
            case 2:
                result.type._double = pow(n1.type._float, n2.type._int);
                break;
            case 3:
                result.type._double = pow(n1.type._float, n2.type._float);
                break;
            case 4:
                result.type._double = pow(n1.type._float, n2.type._double);
                break;
            }
        }
        break;
        case 4:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.type._double = pow(n1.type._double, n2.type._uint32);
                break;
            case 2:
                result.type._double = pow(n1.type._double, n2.type._int);
                break;
            case 3:
                result.type._double = pow(n1.type._double, n2.type._float);
                break;
            case 4:
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
        result.unionCase = 2;
        switch (n1.unionCase)
        {
        case 1:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.type._uint32 = n1.type._uint32 && n2.type._uint32;
                break;
            case 2:
                result.type._int = n1.type._uint32 && n2.type._int;
                break;
            default:
                printf("Error: Invalid AND operation");
                result.type._int = 0;
            }
        }
        break;
        case 2:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 2;
                result.type._int = n1.type._int & n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._int & n2.type._int;
                break;
            default:
                printf("Error: Invalid AND operation");
                result.type._int = 0;
                result.unionCase = 2;
            }
        }
        break;
        default:
            printf("Error: Invalid AND operation");
            result.type._int = 0;
            result.unionCase = 2;
            break;
        }
    }
    break;
    case OR:
    {
        result.unionCase = 2;
        switch (n1.unionCase)
        {
        case 1:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.type._uint32 = n1.type._uint32 || n2.type._uint32;
                break;
            case 2:
                result.type._int = n1.type._uint32 || n2.type._int;
                break;
            default:
                printf("Error: Invalid OR operation");
                result.type._int = 0;
                break;
            }
        }
        break;
        case 2:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.type._int = n1.type._int || n2.type._uint32;
                break;
            case 2:
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
        result.unionCase = 2;
        result.type._int = compare(n1, n2) == -1;
        break;
    }
    case GT:
    {
        result.unionCase = 2;
        result.type._int = compare(n1, n2) == 1;
        break;
    }
    case EQ:
    {
        result.unionCase = 2;
        result.type._int = compare(n1, n2) == 0;
        break;
    }
    case LTEQ:
    {
        result.unionCase = 2;
        result.type._int = compare(n1, n2) <= 0;
        break;
    }
    case GTEQ:
    {
        result.unionCase = 2;
        result.type._int = compare(n1, n2) >= 0;
        break;
    }
    case MOD:
    {
        switch (n1.unionCase)
        {
        case 1:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 % n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._uint32 % n2.type._int;
                break;

            default:
                printf("Error: Invalid MOD operation. Divisor must be integer");
                result.type._int = 0;
                result.unionCase = 2;
                break;
            }
            break;
        }
        case 2:
        {
            switch (n2.unionCase)
            {
            case 1:
                result.unionCase = 2;
                result.type._int = n1.type._int % n2.type._uint32;
                break;
            case 2:
                result.unionCase = 2;
                result.type._int = n1.type._int % n2.type._int;
                break;
            default:
                printf("Error: Invalid MOD operation. Divisor must be integer");
                result.type._int = 0;
                result.unionCase = 2;
                break;
            }
        }
        break;
        default:
            printf("Error: Invalid MOD operation. Dividend must be integer");
            result.type._int = 0;
            result.unionCase = 2;
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
    case 1:
        value = (double)number.type._uint32;
        break;
    case 2:
        value = (double)number.type._int;
        break;
    case 3:
        value = (double)number.type._float;
        break;
    case 4:
        value = number.type._double;
        break;
    }

    // Perform the operation based on the given instruction
    switch (op)
    {
    case SQRT:
        result.type._double = sqrt(value);
        result.unionCase = 4;
        break;
    case EXP:
        result.type._double = exp(value);
        result.unionCase = 4;
        break;
    case CEIL:
        result.type._int = (int)ceil(value);
        result.unionCase = 2;
        break;
    case FLOOR:
        result.type._int = (int)floor(value);
        result.unionCase = 2;
        break;
    case ROUND:
        result.type._int = value + 0.5;
        result.unionCase = 2;
        break;
    case ABS:
        result.type._double = fabs(value);
        result.unionCase = 4;
        break;
    case NOT:
        result.type._int = !(int)value;
        result.unionCase = 2;
        break;
    case LOG:
        result.type._double = log(value);
        result.unionCase = 4;
        break;
    default:
        printf("Error: Invalid unary operation");
    }

    // Return the result
    return result;
}