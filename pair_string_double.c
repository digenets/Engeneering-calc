#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "pair_string_double.h"

#define ERROR 0.0000001     // погрешность

bool ArePairsEqual(PAIR_STRING_DOUBLE* lhs, PAIR_STRING_DOUBLE* rhs) {
    if (strcmp(lhs->first, rhs->first) == 0 && fabs(lhs->second - rhs->second) < ERROR) {
        return true;
    }
    return false;
}
