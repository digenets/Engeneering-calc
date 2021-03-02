#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "test.h"
#include "pair_string_double.h"
#include "linear_map.h"
#include "calculation.h"
#include "constants.h"
#include "rpn_creating.h"

LINEAR_MAP ParseVariables(char** strings, int number_of_vars) {
    LINEAR_MAP vars_map = CreateMap();
    // add constants
    PAIR_STRING_DOUBLE pi = {"PI", M_PI};
    Insert(&vars_map, &pi);
    PAIR_STRING_DOUBLE e = {"e", M_E};
    Insert(&vars_map, &e);

    char** vars_names = (char**) malloc(sizeof(char*) * number_of_vars);
    char** vars_values = (char**) malloc(sizeof(char*) * number_of_vars);; // value or expression

    // считываем сначала имя переменной и её значение/выражение
    for (int i = 0; i < number_of_vars; ++i) {
        vars_names[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE);
        vars_values[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE);

        int assign_index = 0; // индекс '='
        for (int j = 0; j < strlen(strings[i]); ++j) {
            if (strings[i][j] == '=') {
                assign_index = j;
                break;
            }
        }
        // всё что после '=' - это значение
        sscanf(strings[i], "%s =", vars_names[i]);
        strcpy(vars_values[i], &(strings[i][assign_index + 2]));
    }

    // теперь идем по ним в обратном порядке. если это выражение - сосчитаем его
    for (int i = number_of_vars - 1; i >= 0; --i) {
        double value = 0;
        if (!IsDigit(vars_values[i])) {
            int rpn_objects_counter = 0;
            char** rpn = GetRpn(vars_values[i], &rpn_objects_counter);
            ReplaceWithVarsValues(rpn, rpn_objects_counter, &vars_map);
            value = Calculate(rpn, rpn_objects_counter);
        } else {
            value = atof(vars_values[i]);
        }
        PAIR_STRING_DOUBLE pair = {vars_names[i], value};
        Insert(&vars_map, &pair);
    }
    return vars_map;
}

int main(int argc, char** argv) {
    if (argc > 3 && strcmp(argv[3], TEST) == 0) {
        Test();
        return 0;
    }

    FILE* file_input = fopen(argv[1], "r");

    int number_of_lines;
    fscanf(file_input, "%d\n", &number_of_lines);

    char* expression = (char*) malloc(sizeof(char) * MAX_EXPR_SIZE);
    fgets(expression, MAX_EXPR_SIZE, file_input);
    if (expression[strlen(expression) - 1] == '\n') {
        expression[strlen(expression) - 1] = '\0';
    }

    int number_of_vars = number_of_lines - 1;
    char** input_strings = (char**) malloc(sizeof(char*) * number_of_vars);
    for (int i = 0; i < number_of_vars; ++i) {
        input_strings[i] = (char*) malloc(sizeof(char) * MAX_EXPR_SIZE);
        fgets(input_strings[i], MAX_EXPR_SIZE, file_input);
        int last_index = strlen(input_strings[i]) - 1;
        if (input_strings[i][last_index] == '\n') {
            input_strings[i][last_index] = '\0';
        }
    }
    LINEAR_MAP vars_map = ParseVariables(input_strings, number_of_vars);

    int rpn_objects_counter = 0;
    char** rpn = GetRpn(expression, &rpn_objects_counter);
    ReplaceWithVarsValues(rpn, rpn_objects_counter, &vars_map);
    double result = Calculate(rpn, rpn_objects_counter);
    FILE* file_output = fopen(argv[2], "w");
    fprintf(file_output, "%lf", result);
    return 0;
}
