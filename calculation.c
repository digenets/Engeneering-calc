#include "calculation.h"
#include "stack_double.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <complex.h>
#include <ctype.h>

bool IsDigit(char *string) {
    // если в начале тире - передать туда подстроку без тире
    if (string[0] == '-' && strlen(string) > 1) {
        return IsDigit(&string[1]);
    } else {
        // если в начале не тире
        for (int i = 0; i < strlen(string); ++i) {
            if (isdigit(string[i]) || string[i] == '.') {
                continue;
            } else {
                return false;
            }
        }
    }
    return true;
}

bool IsBinary(char* operator) {
    if (strcmp(operator, "*") == 0 ||
        strcmp(operator, "/") == 0 ||
        strcmp(operator, "+") == 0 ||
        strcmp(operator, "-") == 0 ||
        strcmp(operator, "^") == 0 ||
        strcmp(operator, "pow") == 0) {
        return true;
    } else {
        return false;
    }
}

void ReplaceWithVarsValues(char** rpn, int rpn_objects_num, LINEAR_MAP* vars_map) {
    // заменяем в rpn переменные на соответствующие значения
    for (int i = 0; i < rpn_objects_num; ++i) {
        double value = Find(vars_map, rpn[i]);
        if (!isnan(value)) {
            sprintf(rpn[i], "%lf", value);
        }
    }
}

double Calculate(char** rpn, int rpn_size) {
    STACK_DOUBLE stack_of_numbers = CreateStackDouble();
    for (int i = 0; i < rpn_size; ++i) {
        if (IsDigit(rpn[i])) {
            PushToStackDouble(&stack_of_numbers, atof(rpn[i]));
        } else { // if is operator
            char* operator = rpn[i];
            double result = 0;
            if (IsBinary(operator)) {
                // извлекаем из стека два верхних числа, применяем к ним оператор, затем кладем результат обратно в стек
                double x1, x2;
                x1 = PopFromStackDouble(&stack_of_numbers);
                x2 = PopFromStackDouble(&stack_of_numbers);
                if (strcmp(operator, "+") == 0) {
                    result = x1 + x2;
                } else if (strcmp(operator, "-") == 0) {
                    result = x2 - x1;
                } else if (strcmp(operator, "*") == 0) {
                    result = x1 * x2;
                } else if (strcmp(operator, "/") == 0) {
                    result = x2 / x1;
                } else if (strcmp(operator, "^") == 0) {
                    result = pow(x2, x1);
                } else if (strcmp(operator, "pow") == 0) {
                    result = pow(x2, x1);
                } else {
                    printf("Illegal operator: %s\n", operator);
                    abort();
                }
            } else {
                double operand = PopFromStackDouble(&stack_of_numbers);
                if (strcmp(operator, "~") == 0) {
                    result = (-1) * operand;
                } else if (strcmp(operator, "cos") == 0) {
                    result = cos(operand);
                } else if (strcmp(operator, "sin") == 0) {
                    result = sin(operand);
                } else if (strcmp(operator, "tg") == 0) {
                    result = tan(operand);
                } else if (strcmp(operator, "log") == 0) {
                    result = log10(operand);
                } else if (strcmp(operator, "ln") == 0) {
                    result = log(operand);
                } else if (strcmp(operator, "sqrt") == 0) {
                    result = sqrt(operand);
                } else if (strcmp(operator, "abs") == 0) {
                    result = fabs(operand);
                } else if (strcmp(operator, "exp") == 0) {
                    result = exp(operand);
                } else if (strcmp(operator, "real") == 0) {
                    result = creal(operand);
                } else if (strcmp(operator, "imag") == 0) {
                    result = cimag(operand);
                }
//                else if (strcmp(operator, "mag") == 0) { // operand should be complex
//                    result = cabs(operand);
//                } else if (strcmp(operator, "phase") == 0) {
//                    result = carg(operand);
//                }
                else {
                    printf("Illegal operator: %s\n", operator);
                    abort();
                }
            }
            PushToStackDouble(&stack_of_numbers, result);
        }
    }
    double result = PopFromStackDouble(&stack_of_numbers);
    free(stack_of_numbers.stack);
    return result;
}