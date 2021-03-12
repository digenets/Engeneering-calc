#ifndef ENGINEERING_CALC_LINEAR_MAP_H
#define ENGINEERING_CALC_LINEAR_MAP_H

#include <stddef.h>
#include <stdbool.h>
#include "pair_string_double.h"

#define INITIAL_CAPACITY 10

typedef struct LinearMap {
    PAIR_STRING_DOUBLE* pairs;
    size_t capacity;
    size_t size;
}LINEAR_MAP;

LINEAR_MAP CreateMap();
bool Insert(LINEAR_MAP* map, PAIR_STRING_DOUBLE* pair);
void Erase(LINEAR_MAP* map, PAIR_STRING_DOUBLE* pair);
double Find(LINEAR_MAP* map, char* key);

#endif //ENGINEERING_CALC_LINEAR_MAP_H
