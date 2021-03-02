#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "test.h"
#include "linear_map.h"
#include "calculation.h"
#include "constants.h"
#include "rpn_creating.h"

bool AreDoublesEqual(double d1, double d2) {
    if (fabs(d1 - d2) < ERROR) {
        return false;
    }
    return true;
}

void MapTest() {
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
    if (AreDoublesEqual(find1, expected1)) {
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

void RpnCalculationTest() {
    int size1 = 5;
    char** stack_polish1 = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE);
    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        stack_polish1[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE);
    }

    // 2+2*2 = 6
    strcpy(stack_polish1[0], "2");
    strcpy(stack_polish1[1], "2");
    strcpy(stack_polish1[2], "2");
    strcpy(stack_polish1[3], "*");
    strcpy(stack_polish1[4], "+");
    double result1 = Calculate(stack_polish1, size1);
    double expected1 = 6;
    if (AreDoublesEqual(result1, expected1)) {
        printf("Wrong calculation result 1: %lf, expected: %lf\n", result1, expected1);
    }

    // 1+2^6-2^3*6 = 17
    // 1 2 6 ^ + 2 3 ^ 6 * -
    int size2 = 11;
    char** stack_polish2 = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE);
    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        stack_polish2[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE);
    }
    strcpy(stack_polish2[0], "1");
    strcpy(stack_polish2[1], "2");
    strcpy(stack_polish2[2], "6");
    strcpy(stack_polish2[3], "^");
    strcpy(stack_polish2[4], "+");
    strcpy(stack_polish2[5], "2");
    strcpy(stack_polish2[6], "3");
    strcpy(stack_polish2[7], "^");
    strcpy(stack_polish2[8], "6");
    strcpy(stack_polish2[9], "*");
    strcpy(stack_polish2[10], "-");
    double result2 = Calculate(stack_polish2, size2);
    double expected2 = 17;
    if (AreDoublesEqual(result2, expected2)) {
        printf("Wrong calculation result 2: %lf, expected: %lf\n", result2, expected2);
    }

    // 1.5-0.0004*765-3.333333333 = -2.139333333
    // 1.5 0.0004 765 * - 3.33333333 -
    int size3 = 7;
    char** stack_polish3 = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE);
    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        stack_polish3[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE);
    }
    strcpy(stack_polish3[0], "1.5");
    strcpy(stack_polish3[1], "0.0004");
    strcpy(stack_polish3[2], "765");
    strcpy(stack_polish3[3], "*");
    strcpy(stack_polish3[4], "-");
    strcpy(stack_polish3[5], "3.33333333");
    strcpy(stack_polish3[6], "-");
    double result3 = Calculate(stack_polish3, size3);
    double expected3 = -2.139333333;
    if (AreDoublesEqual(result3, expected3)) {
        printf("Wrong calculation result 3: %lf, expected: %lf\n", result3, expected3);
    }
}

void ReplaceUnaryMinusTest() {
    char* expression = "-1-2";
    expression = ReplaceUnaryMinus(expression);
    char* expected = "~1-2";
    if (strcmp(expected, expression) != 0) {
        printf("Wrong replacement: %s, expected: %s", expression, expected);
        abort();
    }

    char* expression2 = "2 + (-1)";
    expression2 = ReplaceUnaryMinus(expression2);
    char* expected2 = "2 + (~1)";
    if (strcmp(expected2, expression2) != 0) {
        printf("Wrong replacement: %s, expected: %s", expression2, expected2);
        abort();
    }
}

void Test() {
    MapTest();
    RpnCalculationTest();
    ReplaceUnaryMinusTest();
}
