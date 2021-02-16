#ifndef ENGINEERING_CALC_PAIR_STRING_DOUBLE_H
#define ENGINEERING_CALC_PAIR_STRING_DOUBLE_H

#include <stdbool.h>

typedef struct PairStringDouble {
    char* first;
    double second;
}PAIR_STRING_DOUBLE;

bool ArePairsEqual(PAIR_STRING_DOUBLE* lhs, PAIR_STRING_DOUBLE* rhs);

#endif //ENGINEERING_CALC_PAIR_STRING_DOUBLE_H
