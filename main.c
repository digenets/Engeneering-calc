#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "test.h"
#include "pair_string_double.h"
#include "linear_map.h"
#include "calculation.h"

#define MAX_EXPRESSION_SIZE 100
#define MAX_VAR_NAME_SIZE 50

LINEAR_MAP ParseVariables(char** strings, int number_of_vars) {
    LINEAR_MAP map = CreateMap();
    for (int i = 0; i < number_of_vars; ++i) {
        double value;
        char* name = (char*) malloc(sizeof(char) * MAX_VAR_NAME_SIZE);

        sscanf(strings[i], "%s = %lf", name, &value);
        PAIR_STRING_DOUBLE pair = {name, value};
        Insert(&map, &pair);
    }
    return map;
}

int main(int argc, char** argv) {
    if (argc > 3 && strcmp(argv[3], TEST) == 0) {
        Test();
        return 0;
    }

    FILE* file_input = fopen(argv[1], "r");

    int number_of_lines;
    fscanf(file_input, "%d\n", &number_of_lines);

    char* expression = (char*) malloc(sizeof(char) * MAX_EXPRESSION_SIZE); // expression
    fgets(expression, MAX_EXPRESSION_SIZE, file_input);
    if (expression[strlen(expression) - 1] == '\n') {
        expression[strlen(expression) - 1] = '\0';
    }

    int number_of_vars = number_of_lines - 1;
    char** input_strings = (char**) malloc(sizeof(char*) * number_of_vars);
    for (int i = 0; i < number_of_vars; ++i) {
        input_strings[i] = (char*) malloc(sizeof(char) * MAX_EXPRESSION_SIZE);
        fgets(input_strings[i], MAX_EXPRESSION_SIZE, file_input);
    }
    LINEAR_MAP vars_map = ParseVariables(input_strings, number_of_vars);


    int rpn_objects_counter = 0;
    for (int i = 0; rpn[i][0] != '\0'; i++) {
        rpn_objects_counter++;
    }

    // rpn получена
    // заменяем в ней переменные на соответствующие значения
    for (int i = 0; i < rpn_objects_counter; ++i) {
        double value = Find(&vars_map, rpn[i]);
        if (!isnan(value)) {
            sprintf(rpn[i], "%lf", value);
        }
    }
    double result = Calculate(rpn, rpn_objects_counter);
    FILE* file_output = fopen(argv[2], "w");
    fprintf(file_output, "%lf", result);
    return 0;
}
