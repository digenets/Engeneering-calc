#include "stack_double.h"
#include <stdlib.h>

STACK_DOUBLE CreateStackDouble() {
    double* stack = (double*) malloc(sizeof(double) * MAX_STACK_SIZE);
    int free_index = 0;
    STACK_DOUBLE stack_struct = {stack, free_index};
    return stack_struct;
}

bool PushToStackDouble(STACK_DOUBLE* stack, double num) {
    if (stack->free_index < MAX_STACK_SIZE) {
        stack->stack[stack->free_index] = num;
        ++(stack->free_index);
        return true;
    }
    return false;
}

bool IsEmptyStackDouble(STACK_DOUBLE* stack) {
    if (stack->free_index == 0) {
        return true;
    }
    return false;
}

double PopFromStackDouble(STACK_DOUBLE* stack) {
    if (!IsEmptyStackDouble(stack)) {
        --(stack->free_index);
    }
    return stack->stack[stack->free_index];
}