#ifndef ENGINEERING_CALC_STACK_STRINGS_H
#define ENGINEERING_CALC_STACK_STRINGS_H

#include "constants.h"

int push(char** stack, int cur, char str[MAX_ELEMENT_SIZE]);
int is_empty(char** stack, unsigned cur);
char top(char** stack, unsigned cur);
int pop(char** stack, unsigned cur);

#endif //ENGINEERING_CALC_STACK_STRINGS_H
