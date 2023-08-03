#include <stdio.h>
#include <math.h>
#include "operators.h"

Number bin_op(Number n1, Number n2, ExpressionInstruction op){
    Number result;
    if (op == ADD) {
        if(n1.unionCase == 1){
            if(n2.unionCase == 1){
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 + n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._uint32 + n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._uint32 + n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._uint32 + n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 2){
            if(n2.unionCase == 1){
                result.unionCase = 2;
                result.type._int = n1.type._int + n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._int + n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._int + n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._int + n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 3){
            if(n2.unionCase == 1){
                result.unionCase = 3;
                result.type._float = n1.type._float + n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 3;
                result.type._float = n1.type._float + n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._float + n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._float + n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 4){
            if(n2.unionCase == 1){
                result.unionCase = 4;
                result.type._double = n1.type._double + n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 4;
                result.type._double = n1.type._double + n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 4;
                result.type._double = n1.type._double + n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._double + n2.type._double;
                return result;
            }
        }
    } else if(op == SUB){
        if(n1.unionCase == 1){
            if(n2.unionCase == 1){
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 - n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._uint32 - n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._uint32 - n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._uint32 - n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 2){
            if(n2.unionCase == 1){
                result.unionCase = 2;
                result.type._int = n1.type._int - n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._int - n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._int - n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._int - n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 3){
            if(n2.unionCase == 1){
                result.unionCase = 3;
                result.type._float = n1.type._float - n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 3;
                result.type._float = n1.type._float - n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._float - n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._float - n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 4){
            if(n2.unionCase == 1){
                result.unionCase = 4;
                result.type._double = n1.type._double - n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 4;
                result.type._double = n1.type._double - n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 4;
                result.type._double = n1.type._double - n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._double - n2.type._double;
                return result;
            }
        }
    } else if(op == MUL){
        if(n1.unionCase == 1){
            if(n2.unionCase == 1){
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 * n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._uint32 * n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._uint32 * n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._uint32 * n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 2){
            if(n2.unionCase == 1){
                result.unionCase = 2;
                result.type._int = n1.type._int * n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._int * n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._int * n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._int * n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 3){
            if(n2.unionCase == 1){
                result.unionCase = 3;
                result.type._float = n1.type._float * n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 3;
                result.type._float = n1.type._float * n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 3;
                result.type._float = n1.type._float * n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._float * n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 4){
            if(n2.unionCase == 1){
                result.unionCase = 4;
                result.type._double = n1.type._double * n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 4;
                result.type._double = n1.type._double * n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 4;
                result.type._double = n1.type._double * n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = n1.type._double * n2.type._double;
                return result;
            }
        }
    } else if(op == DIV){
        if(n1.unionCase == 1){
            if(n2.unionCase == 1){
                if(n2.type._uint32 == 0){
                    printf("Error: Division by zero");
                    result.type._uint32 = 0;
                    return result;
                }
                result.unionCase = 1;
                result.type._uint32 = n1.type._uint32 / n2.type._uint32; 
                return result;
            } else if (n2.unionCase == 2){
                if(n2.type._int == 0){
                    printf("Error: Division by zero");
                    result.type._uint32 = 0;
                    return result;
                }
                result.unionCase = 2;
                result.type._int = n1.type._uint32 / n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                if(n2.type._float == 0){
                    printf("Error: Division by zero");
                    result.type._uint32 = 0;
                    return result;
                }
                result.unionCase = 3;
                result.type._float = n1.type._uint32 / n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                if(n2.type._double == 0){
                    printf("Error: Division by zero");
                    result.type._uint32 = 0;
                    return result;
                }
                result.unionCase = 4;
                result.type._double = n1.type._uint32 / n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 2){
            if(n2.unionCase == 1){
                if(n2.type._uint32 == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 2;
                result.type._int = n1.type._int / n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                if(n2.type._int == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 2;
                result.type._int = n1.type._int / n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                if(n2.type._float == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 3;
                result.type._float = n1.type._int / n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                if(n2.type._double == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 4;
                result.type._double = n1.type._int / n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 3){
            if(n2.unionCase == 1){
                if(n2.type._uint32 == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 3;
                result.type._float = n1.type._float / n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                if(n2.type._int == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 3;
                result.type._float = n1.type._float / n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                if(n2.type._float == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 3;
                result.type._float = n1.type._float / n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                if(n2.type._double == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 4;
                result.type._double = n1.type._float / n2.type._double;
                return result;
            }
        } else if(n1.unionCase == 4){
            if(n2.unionCase == 1){
                if(n2.type._uint32 == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 4;
                result.type._double = n1.type._double / n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                if(n2.type._int == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 4;
                result.type._double = n1.type._double / n2.type._int;
                return result;
            } else if (n2.unionCase == 3){
                if(n2.type._float == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 4;
                result.type._double = n1.type._double / n2.type._float;
                return result;
            } else if (n2.unionCase == 4){
                if(n2.type._double == 0){
                    printf("Error: Division by zero");
                    result.type._float = 0;
                    return result;
                }
                result.unionCase = 4;
                result.type._double = n1.type._double / n2.type._double;
                return result;
            }
        }
    } else if (op == POW){
        if(n1.unionCase == 1){
            if(n2.unionCase == 1){
                result.unionCase = 4;
                result.type._double = pow((double) n1.type._uint32, (double)n2.type._uint32);
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._uint32,(double) n2.type._int);
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._uint32, (double)n2.type._float);
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._uint32, n2.type._double);
                return result;
            }
        } else if(n2.unionCase == 2){
            if(n2.unionCase == 1){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._int, (double)n2.type._uint32);
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._int, (double)n2.type._int);
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._int, (double)n2.type._float);
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._int, n2.type._double);
                return result;
            }
        } else if(n1.unionCase == 3){
            if(n2.unionCase == 1){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._float, (double)n2.type._uint32);
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._float,(double) n2.type._int);
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._float, (double)n2.type._float);
                return result;
            } else if (n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = pow((double)n1.type._float, n2.type._double);
                return result;
            }
        } else if(n1.unionCase == 4){
            if(n2.unionCase == 1){
                result.unionCase = 4;
                result.type._double = pow(n1.type._double,(double) n2.type._uint32);
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 4;
                result.type._double = pow(n1.type._double,(double) n2.type._int);
                return result;
            } else if (n2.unionCase == 3){
                result.unionCase = 4;
                result.type._double = pow(n1.type._double, (double)n2.type._float);
                return result;
            } else if(n2.unionCase == 4){
                result.unionCase = 4;
                result.type._double = pow(n1.type._double, n2.type._double);
                return result;
            }
        }
    } else if(op == AND){
        if(n1.unionCase == 1){
            if(n2.unionCase == 1){
                result.unionCase = 2;
                result.type._int = n1.type._uint32 & n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._uint32 & n2.type._int;
                return result;
            } else {
                printf("Error: Invalid AND operation");
                result.type._int = 0;
                result.unionCase = 2;
                return result;
            }
        } else if(n1.unionCase == 2){
            if(n2.unionCase == 1){
                result.unionCase = 2;
                result.type._int = n1.type._int & n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._int & n2.type._int;
                return result;
            } else {
                printf("Error: Invalid AND operation");
                result.type._int = 0;
                result.unionCase = 2;
                return result;
            }
        } else {
            printf("Error: Invalid AND operation");
            result.type._int = 0;
            result.unionCase = 2;
            return result;
        }
    } else if(op == OR){
        if(n1.unionCase == 1){
            if(n2.unionCase == 1){
                result.unionCase = 2;
                result.type._int = n1.type._uint32 | n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._uint32 | n2.type._int;
                return result;
            } else {
                printf("Error: Invalid OR operation");
                result.type._int = 0;
                result.unionCase = 2;
                return result;
            }
        } else if(n1.unionCase == 2){
            if(n2.unionCase == 1){
                result.unionCase = 2;
                result.type._int = n1.type._int | n2.type._uint32;
                return result;
            } else if (n2.unionCase == 2){
                result.unionCase = 2;
                result.type._int = n1.type._int | n2.type._int;
                return result;
            } else {
                printf("Error: Invalid OR operation");
                result.type._int = 0;
                result.unionCase = 2;
                return result;
            }
        } else {
            printf("Error: Invalid OR operation");
            result.type._int = 0;
            result.unionCase = 2;
            return result;
        }
    }
    result.type._int = 0;
    result.unionCase = 2;
    return result;
}

Number un_op(Number number, ExpressionInstruction op){
    Number result;
    double value = 0.0;

    // Extract the value from the Number union
    switch (number.unionCase) {
        case 1:
            value = (double) number.type._uint32;
            break;
        case 2:
            value = (double) number.type._int;
            break;
        case 3:
            value = (double) number.type._float;
            break;
        case 4:
            value = number.type._double;
            break;
    }
    
 // Perform the operation based on the given instruction
    switch (op) {
        case SQRT:
            result.type._double = sqrt(value);
            result.unionCase = 4;
            break;
        case EXP:
            result.type._double = exp(value);
            result.unionCase = 4;
            break;
        case CEIL:
            result.type._int = (int) ceil(value);
            result.unionCase = 2;
            break;
        case FLOOR:
            result.type._int = (int) floor(value);
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
            result.type._int = !(int) value;
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