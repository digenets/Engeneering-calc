#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include "test.h"
#include "pair_string_double.h"
#include "linear_map.h"

#define MAX_EXPRESSION_SIZE 100
#define MAX_VAR_NAME_SIZE 50
#define SIZE1 128
#define SIZE2 256
#define MAX_SIZE 1024

int is_oper (char *str, int i) {
    if (str[i] == '^') {
        return 3;
    }
    if ((str[i] == '*') || (str[i] == '/')) {
        return 2;
    }
    if ((str[i] == '+') || (str[i] == '-')) {
        return 1;
    }
    return 0;
}

int push(char stack[MAX_SIZE][256], int cur, char str[256]) {
    if (cur < MAX_SIZE)
    {
        strcpy(stack[cur], str);
        cur++;
    }
    return cur;
}

int is_empty(const char stack[MAX_SIZE][256], unsigned cur) {
    return !cur;
}

char top(const char stack[MAX_SIZE][256], unsigned cur) {
    if (!is_empty(stack, cur))
        return stack[cur - 1][0];
    return 0;
}

int pop(char stack[MAX_SIZE][256], unsigned cur){
    if(!is_empty(stack, cur))
        --cur;
    return cur;
}


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
    char expressions[SIZE1][SIZE2] = {'\0'};
    int expressions_results[SIZE1] = {0};
    int expressions_count = 0;
    char variables[SIZE1][SIZE2] = {'\0'};
    char variables_names[SIZE1][SIZE2] = {'\0'};
    int variables_count = 0;
    char functions[13][SIZE2] = {'\0'};
    char funcs_names[13][SIZE2] = {
            "cos",
            "sin",
            "tg",
            "log",
            "ln",
            "sqrt",
            "pow",
            "abs",
            "exp",
            "real",
            "imag",
            "mag",
            "phase"
    };
    char str[256] = "5^765-abc*65565-6+5\n";
    int abc = 5;
    int current_rpn = 0;
    char rpn[MAX_SIZE][256];
    int current_operator = 0;
    char oper_stack[MAX_SIZE][256];

    if (argc > 1 && strcmp(argv[1], TEST) == 0) {
        Test();
        return 0;
    }

    FILE* file_input = fopen(argv[1], "r");

    int number_of_lines;
    fscanf(file_input, "%d\n", &number_of_lines);

    char* expression = (char*) malloc(sizeof(char) * MAX_EXPRESSION_SIZE);
    fgets(expression, MAX_EXPRESSION_SIZE,  file_input);

    int number_of_numbers = number_of_lines - 1;
    char** input_strings = (char**) malloc(sizeof(char*) * number_of_numbers);
    for (int i = 0; i < number_of_numbers; ++i) {
        input_strings[i] = (char*) malloc(sizeof(char) * MAX_EXPRESSION_SIZE);
        fgets(input_strings[i], MAX_EXPRESSION_SIZE, file_input);
    }
    LINEAR_MAP vars_map = ParseVariables(input_strings, number_of_numbers);

    for (int i = 0; i < strlen(str) - 1; i++) {
        char token[256] = {'\0' };
        char function[256] = {'\0' };
        bool is_function = 0;
        bool is_variable = 0;
        int k = 0;

        if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')) {
            for (int j = i; j < strlen(str) - 1; j++) {
                if (is_oper(str, j)) {
                    break;
                }
                if (str[j] == ')') {//WARNING
                    break;
                }
                token[k] = str[j];
                k++;
            }
            for (int n = 0; n < strlen(token); n++) {
                if (token[n] == '(') {
                    is_function = 1;
                    is_variable = 0;
                    break;
                }
            }
            if (!is_function) {
                is_function = 0;
                is_variable = 1;
            }
            else {
                strcpy(function, token);
            }
        }
        if (!is_function && is_variable) {
            current_rpn = push(rpn, current_rpn, token); // function "push" return current_rpn++
            i = i + k - 1;
        }
        else if (is_function) {
            // expon(23424+3423+2)
            //count number of letters in name of function
            int c = 0;
            for (int j = 0; j < strlen(function); j++) {
                if (function[j] == 40) {
                    c++;
                    break;
                }
                c++;
            }
            i = i + c - 2;
            for (int j = 0; j < strlen(function); j++) {
                if (j < c - 1) {
                    function[j] = function[j];
                } else {
                    function[j] = '\0';
                }
            }
            current_operator = push(oper_stack, current_operator, function);
        }
        else if (is_oper(str, i)) {
            char temporary_top = top(oper_stack, current_operator);
            char token_top[256] = {'\0'};
            token_top[0] = temporary_top;
            // Check that top of stack is operator
            while  ((top(oper_stack, current_operator) && is_oper(token_top, 0)) // пока стек не пустой и в топе оператор
                     &&

                    (
                             /*либо*/((is_oper(str, i) == 1 || is_oper(str, i) == 2)/*токен левоасоц*/ && (is_oper(str, i) <= is_oper(token_top, 0))/*и приоритет токена меньше либо равен топу*/)
                                     ||
                                     /*либо*/((is_oper(str, i) == 3) /*токен правоасоц*/ && (is_oper(str, i) < is_oper(token_top, 0)) /*и приоритет токена меньше топа*/)
                     )

                    && is_oper(token_top, 0) != 40
                    )
            {//while cicle
                temporary_top = top(oper_stack, current_operator);
                token_top[0] = temporary_top;
                current_rpn = push(rpn, current_rpn, token_top);
                current_operator = pop(oper_stack, current_operator);
                //retake
                temporary_top = top(oper_stack, current_operator);
                for (int j = 0; j < 256; j++) {
                    token_top[j] = '\0';
                }
                token_top[0] = temporary_top;
            }
            //end of while cicle

            char token_oper[256] = {'\0' };
            token_oper[0] = str[i];
            current_operator = push(oper_stack, current_operator, token_oper);
        }
        else if (str[i] == 40) {
            char left_parentheses[256] = {'\0'};
            left_parentheses[0] = 40; //str where first element is "("
            current_operator = push(oper_stack, current_operator, left_parentheses);
        }
        else if (str[i] == 41) {
            while (top(oper_stack, current_operator) != '(') {
                char token_top[256] = {'\0'};
                char tmp_top = top(oper_stack, current_operator);
                token_top[0] = tmp_top;
                current_rpn = push(rpn, current_rpn, token_top);
                current_operator = pop(oper_stack, current_operator);
            }
            if (top(oper_stack, current_operator) == '(') {
                current_operator = pop(oper_stack, current_operator);
            }
            if ((top(oper_stack, current_operator) >= 'A' && top(oper_stack, current_operator) <= 'Z') || (top(oper_stack, current_operator) >= 'a' && top(oper_stack, current_operator) <= 'z')) {
                current_rpn = push(rpn, current_rpn, oper_stack[current_operator - 1]);
                current_operator = pop(oper_stack, current_operator);
            }
        }
    }
    while (!is_empty(oper_stack, current_operator)) {
        char top_oper[256];
        top_oper[0] = top(oper_stack, current_operator);
        current_rpn = push(rpn, current_rpn, top_oper);
        current_operator = pop(oper_stack, current_operator);
    }

    printf("{");
    for (int i = 0; i < current_rpn; i++) {
        printf("%s,",rpn[i]);
    }
    printf("}\n");

    return 0;
}
