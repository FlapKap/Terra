#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "number.h"

#include <debug.h>

int compare(Number n1, Number n2){
    if(n1.unionCase == 1){
        if(n2.unionCase == 1){
            if(n1.type._uint32 > n2.type._uint32){
                return 1;
            } else if (n1.type._uint32 < n2.type._uint32){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 2){
            if((int)(n1.type._uint32) > n2.type._int){
                return 1;
            } else if ((int)(n1.type._uint32) < n2.type._int){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 3){
            if(n1.type._uint32 > n2.type._float){
                return 1;
            } else if (n1.type._uint32 < n2.type._float){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 4){
            if(n1.type._uint32 > n2.type._double){
                return 1;
            } else if (n1.type._uint32 < n2.type._double){
                return -1;
            } else {
                return 0;
            }
        }
    } else if(n1.unionCase == 2){
        if(n2.unionCase == 1){
            //if an int is less than 0 it is less than a uint32
            if(n1.type._int < 0){
                return -1;
            }
            if(n1.type._int > (int)(n2.type._uint32)){
                return 1;
            } else if (n1.type._int < (int)(n2.type._uint32)){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 2){
            if(n1.type._int > n2.type._int){
                return 1;
            } else if (n1.type._int < n2.type._int){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 3){
            if(n1.type._int > n2.type._float){
                return 1;
            } else if (n1.type._int < n2.type._float){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 4){
            if(n1.type._int > n2.type._double){
                return 1;
            } else if (n1.type._int < n2.type._double){
                return -1;
            } else {
                return 0;
            }
        }
    } else if(n1.unionCase == 3){
        if(n2.unionCase == 1){
            if(n1.type._float > n2.type._uint32){
                return 1;
            } else if (n1.type._float < n2.type._uint32){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 2){
            if(n1.type._float > n2.type._int){
                return 1;
            } else if (n1.type._float < n2.type._int){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 3){
            if(n1.type._float > n2.type._float){
                return 1;
            } else if (n1.type._float < n2.type._float){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 4){
            if(n1.type._float > n2.type._double){
                return 1;
            } else if (n1.type._float < n2.type._double){
                return -1;
            } else {
                return 0;
            }
        }
    } else if(n1.unionCase == 4){
        if(n2.unionCase == 1){
            if(n1.type._double > n2.type._uint32){
                return 1;
            } else if (n1.type._double < n2.type._uint32){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 2){
            if(n1.type._double > n2.type._int){
                return 1;
            } else if (n1.type._double < n2.type._int){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 3){
            if(n1.type._double > n2.type._float){
                return 1;
            } else if (n1.type._double < n2.type._float){
                return -1;
            } else {
                return 0;
            }
        } else if (n2.unionCase == 4){
            if(n1.type._double > n2.type._double){
                return 1;
            } else if (n1.type._double < n2.type._double){
                return -1;
            } else {
                return 0;
            }
        }
    }
    return 0;
}

Number bin_op(Number n1, Number n2, char op){
    Number result;
    if (op == '+') {
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
    } else if(op == '-'){
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
    } else if(op == '*'){
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
    } else if(op == '/'){
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
    } else if (op == '^'){
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
    } else if(op == '&'){
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
    } else if(op == '|'){
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

// TODO: Can we be more efficient than strcmp?
Number un_op(Number n1, char* op){
    Number result;
    double val = 0.0;

    if (n1.unionCase == 1) {
        val = (double) n1.type._uint32;
    } else if (n1.unionCase == 2) {
        val = (double) n1.type._int;
    } else if (n1.unionCase == 3) {
        val = (double) n1.type._float;
    } else if (n1.unionCase == 4) {
        val = n1.type._double;
    }
    
    if(strcmp(op, "log") == 0){
        result.type._double = log(val);
        result.unionCase = 4;
    } else if(strcmp(op, "sqrt") == 0){
        result.type._double = sqrt(val);
        result.unionCase = 4;
    } else if(strcmp(op, "exp") == 0){
        result.type._double = exp(val);
        result.unionCase = 4;
    } else if(strcmp(op, "ceil") == 0){
        result.type._int = (int) ceil(val);
        result.unionCase = 2;
    } else if(strcmp(op, "floor") == 0){
        result.type._int = (int) floor(val);
        result.unionCase = 2;
    } else if(strcmp(op, "round") == 0){
        result.type._int = val + 0.5;
        result.unionCase = 2;
    } else if(strcmp(op, "abs") == 0){
        result.type._double = fabs(val);
        result.unionCase = 4;
    } else if(strcmp(op, "not") == 0) {
        result.type._int = !(int)val;
        result.unionCase = 2;
    }

    return result;
}