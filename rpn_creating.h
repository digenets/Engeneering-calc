#ifndef ENGINEERING_CALC_RPN_CREATING_H
#define ENGINEERING_CALC_RPN_CREATING_H
#include "linear_map.h"
char* ReplaceUnaryMinus(char* expression);
char** GetRpn(char* expression, int* rpn_objects_number, LINEAR_MAP* vars_map);

#endif //ENGINEERING_CALC_RPN_CREATING_H
