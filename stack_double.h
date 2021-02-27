#ifndef ENGINEERING_CALC_STACK_DOUBLE_H
#define ENGINEERING_CALC_STACK_DOUBLE_H

#include <stdbool.h>

#define MAX_STACK_SIZE 10000

typedef struct StackDouble {
    double* stack;
    int free_index;
}STACK_DOUBLE;

STACK_DOUBLE CreateStackDouble();
bool PushToStackDouble(STACK_DOUBLE* stack, double num);
bool IsEmptyStackDouble(STACK_DOUBLE* stack);
double PopFromStackDouble(STACK_DOUBLE* stack);

#endif //ENGINEERING_CALC_STACK_DOUBLE_H
