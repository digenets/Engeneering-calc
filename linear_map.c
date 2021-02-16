#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "linear_map.h"

LINEAR_MAP CreateMap() {
    int capacity = 10;
    PAIR_STRING_DOUBLE* pairs = (PAIR_STRING_DOUBLE*) malloc(sizeof(PAIR_STRING_DOUBLE) * capacity);
    LINEAR_MAP map;
    map.pairs = pairs;
    map.capacity = capacity;
    map.size = 0;
    return map;
}

void Reallocate(LINEAR_MAP* map) {
    size_t new_capacity = map->capacity * 2;
    PAIR_STRING_DOUBLE* new_pairs_array = (PAIR_STRING_DOUBLE*) malloc(sizeof(PAIR_STRING_DOUBLE) * new_capacity);
    memcpy(new_pairs_array, map->pairs, sizeof(PAIR_STRING_DOUBLE) * map->size);

    free(map->pairs);
    map->pairs = new_pairs_array;
    map->capacity = new_capacity;
}

bool Insert(LINEAR_MAP* map, PAIR_STRING_DOUBLE* pair) {
    if (map->size == map->capacity) {
        Reallocate(map);
    }

    for (int i = 0; i < map->size; ++i) {
        if (ArePairsEqual(&map->pairs[i], pair)) {
            return false;
        }
    }

    map->pairs[map->size] = *pair;
    ++map->size;
    return true;
}

void Erase(LINEAR_MAP* map, PAIR_STRING_DOUBLE* pair) {
    int index = -1;
    for (int i = 0; i < map->size; ++i) {
        if (strcmp(map->pairs[i].first, pair->first) == 0) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index + 1; i < map->size; ++i) {
            map->pairs[i - 1] = map->pairs[i];
        }
        --map->size;
    }
}

double Find(LINEAR_MAP* map, char* key) {
    for (int i = 0; i < map->size; ++i) {
        if (strcmp(map->pairs[i].first, key) == 0) {
            return map->pairs[i].second;
        }
    }
    return NAN;
}