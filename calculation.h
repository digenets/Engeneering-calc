#ifndef ENGINEERING_CALC_CALCULATION_H
#define ENGINEERING_CALC_CALCULATION_H

#include <stdbool.h>
#include "constants.h"
#include "linear_map.h"

bool IsDigit(char *string);
void ReplaceWithVarsValues(char** rpn, int rpn_objects_num, LINEAR_MAP* vars_map);
double Calculate(char** rpn, int rpn_size);

#endif //ENGINEERING_CALC_CALCULATION_H
