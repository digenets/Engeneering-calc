#include "calculation.h"
#include "stack_double.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

bool IsDigit(char *string) {
    if ((strlen(string) > 1 && string[0] == '-') || (string[0] >= '0' && string[0] <= '9')) {
        return true;
    } else {
        return false;
    }
}

double Calculate(char rpn[1024][256], int rpn_size) {
    STACK_DOUBLE stack_of_numbers = CreateStackDouble();
    for (int i = 0; i < rpn_size; ++i) {
        if (IsDigit(rpn[i])) {
            PushToStackDouble(&stack_of_numbers, atof(rpn[i]));
        } else { // if is operator
            // извлекаем из стека два верхних числа, применяем к ним оператор, затем кладем результат обратно в стек
            double x1, x2;
            x1 = PopFromStackDouble(&stack_of_numbers);
            x2 = PopFromStackDouble(&stack_of_numbers);
            double result = 0;
            if (strcmp(rpn[i], "+") == 0) {
                result = x1 + x2;
            } else if (strcmp(rpn[i], "-") == 0) {
                result = x2 - x1;
            } else if (strcmp(rpn[i], "*") == 0) {
                result = x1 * x2;
            } else if (strcmp(rpn[i], "/") == 0) {
                result = x2 / x1;
            } else if (strcmp(rpn[i], "^") == 0) {
                result = pow(x2, x1);
            } else {
                printf("Illegal operator: %s\n", rpn[i]);
                abort();
            }
            PushToStackDouble(&stack_of_numbers, result);
        }
    }
    double result = PopFromStackDouble(&stack_of_numbers);
    free(stack_of_numbers.stack);
    return result;
}