#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "test.h"
#include "linear_map.h"

void Test() {
    // TEST 1 (map works)
    LINEAR_MAP map = CreateMap();
    PAIR_STRING_DOUBLE pair1 = {"one", 1};
    PAIR_STRING_DOUBLE pair2 = {"two", 2};
    PAIR_STRING_DOUBLE pair3 = {"three", 3};
    PAIR_STRING_DOUBLE pair4 = {"one", 1};

    Insert(&map, &pair1);
    Insert(&map, &pair2);
    Insert(&map, &pair3);
    Insert(&map, &pair4);
    size_t expected_size = 3;
    if (map.size != expected_size) {
        printf("Size after inserting: %zu, expected: %zu\n", map.size, expected_size);
        abort();
    }

    double find1 = Find(&map, "three");
    double expected1 = 3;
    if (fabs(find1 - expected1) > ERROR) {
        printf("Found:\n%lf\n", find1);
        printf("Expected:\n%lf\n", expected1);
        abort();
    }
    double find2 = Find(&map, "tree");
    double expected2 = NAN;
    if (!isnan(find2)) {
        printf("Found:\n%lf\n", find2);
        printf("Expected:\n%lf\n", expected2);
        abort();
    }

    Erase(&map, &pair1);
    size_t size_after_erasing = 2;
    if (map.size != size_after_erasing) {
        printf("Size after erasing: %zu, expected: %zu\n", map.size, size_after_erasing);
        abort();
    }
}