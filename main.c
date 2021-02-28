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

int is_oper (char c) {
    if (c == '^' || c == '~') {
        return 3;
    }
    if ((c == '*') || (c == '/')) {
        return 2;
    }
    if ((c == '+') || (c == '-')) {
        return 1;
    }
    return 0;
}

int push(char stack[MAX_RPN_SIZE][MAX_ELEMENT_SIZE], int cur, char str[MAX_ELEMENT_SIZE]) {
    if (cur < MAX_RPN_SIZE)
    {
        strcpy(stack[cur], str);
        cur++;
    }
    return cur;
}

int is_empty(const char stack[MAX_RPN_SIZE][MAX_ELEMENT_SIZE], unsigned cur) {
    return !cur;
}

char top(const char stack[MAX_RPN_SIZE][MAX_ELEMENT_SIZE], unsigned cur) {
    if (!is_empty(stack, cur))
        return stack[cur - 1][0];
    return 0;
}

int pop(char stack[MAX_RPN_SIZE][MAX_ELEMENT_SIZE], unsigned cur){
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
    expression = ReplaceUnaryMinus(expression);

    int number_of_vars = number_of_lines - 1;
    char** input_strings = (char**) malloc(sizeof(char*) * number_of_vars);
    for (int i = 0; i < number_of_vars; ++i) {
        input_strings[i] = (char*) malloc(sizeof(char) * MAX_EXPR_SIZE);
        fgets(input_strings[i], MAX_EXPR_SIZE, file_input);
    }
    LINEAR_MAP vars_map = ParseVariables(input_strings, number_of_vars);
    PAIR_STRING_DOUBLE pi = {"PI", M_PI};
    Insert(&vars_map, &pi);
    PAIR_STRING_DOUBLE e = {"e", M_E};
    Insert(&vars_map, &e);

    int cur_rpn = 0;
    char rpn[MAX_RPN_SIZE][MAX_ELEMENT_SIZE];
    int cur_oper_stack = 0;
    char oper_stack[MAX_RPN_SIZE][MAX_ELEMENT_SIZE];

    for (int i = 0; i < strlen(expression); i++) {
        int is_func = 0;
        int is_variable = 0;
        int k = 0;
        char token[MAX_ELEMENT_SIZE] = {'\0' }; // variable, function name or a number
        char tmp_func_str[MAX_ELEMENT_SIZE] = { '\0' };
        if ((expression[i] >= '0' && expression[i] <= '9')
            || (expression[i] >= 'A' && expression[i] <= 'Z')
            || (expression[i] >= 'a' && expression[i] <= 'z')) { // not an operand
            for (int j = i; j < strlen(expression); j++) {
                if (is_oper(expression[j])) {
                    break;
                }
                if (expression[j] == ')') {//WARNING
                    break;
                }
                token[k] = expression[j];
                k++;
            }

            // если имя переменной заканчивается пробелом, убираем его
            if (token[strlen(token) - 1] == ' ') {
                token[strlen(token) - 1] = '\0';
            }

            for (int n = 0; n < strlen(token); n++) {
                if (token[n] == '(') {
                    is_func = 1;
                    is_variable = 0;
                    break;
                }
            }
            if (!is_func) {
                is_func = 0;
                is_variable = 1;
            } else {
                strcpy(tmp_func_str, token);
            }
        }

        if (!is_func && is_variable) {
            cur_rpn = push(rpn, cur_rpn, token); // function "push" return cur_rpn++
            i = i + k - 1;
        } else if (is_func) {
            //count number of letters in name of function
            int counter = 0;
            for (int j = 0; j < strlen(tmp_func_str); j++) {
                counter++;
                if (tmp_func_str[j] == '(') {
                    break;
                }
            }
            i = i + counter - 2;
            // add '\0' to the end of string
            for (int j = 0; j < strlen(tmp_func_str); j++) {
                if (j >= counter - 1) {
                    tmp_func_str[j] = '\0';
                }
            }
            cur_oper_stack = push(oper_stack, cur_oper_stack, tmp_func_str);
        } else if (is_oper(expression[i])) {
            char tmp_top = top(oper_stack, cur_oper_stack);
            char str_tmp_top[MAX_ELEMENT_SIZE] = {'\0'};
            str_tmp_top[0] = tmp_top;
            // Check that top of stack is operator
            while  ((!is_empty(oper_stack, cur_oper_stack) && is_oper(str_tmp_top[0])) // пока стек не пустой и в топе оператор
                    &&

                    (
                            /*либо*/((is_oper(expression[i]) == 1 || is_oper(expression[i]) == 2)/*токен левоасоц*/ && (is_oper(expression[i]) <= is_oper(str_tmp_top[0]))/*и приоритет токена меньше либо равен топу*/)
                                    ||
                                    /*либо*/((is_oper(expression[i]) == 3) /*токен правоасоц*/ && (is_oper(expression[i]) < is_oper(str_tmp_top[0])) /*и приоритет токена меньше топа*/)
                    )

                    && str_tmp_top[0] != '('
                    )
            {//while cycle
                tmp_top = top(oper_stack, cur_oper_stack);
                str_tmp_top[0] = tmp_top;
                cur_rpn = push(rpn, cur_rpn, str_tmp_top);
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
                //retake
                tmp_top = top(oper_stack, cur_oper_stack);
                memset(str_tmp_top, '\0', strlen(str_tmp_top));
                str_tmp_top[0] = tmp_top;
            }
            //end of while cycle

            char str_tmp_oper[MAX_ELEMENT_SIZE] = { '\0' };
            str_tmp_oper[0] = expression[i];
            cur_oper_stack = push(oper_stack, cur_oper_stack, str_tmp_oper);
        }
        else if (expression[i] == '(') {
            char open_bracket_str[MAX_ELEMENT_SIZE] = {'\0'};
            open_bracket_str[0] = '('; //str where first element is "("
            cur_oper_stack = push(oper_stack, cur_oper_stack, open_bracket_str);
        }
        else if (expression[i] == ')') {
            while (top(oper_stack, cur_oper_stack) != '(') {
                char str_tmp_top[MAX_ELEMENT_SIZE] = {'\0'};
                char tmp_top = top(oper_stack, cur_oper_stack);
                str_tmp_top[0] = tmp_top;
                cur_rpn = push(rpn, cur_rpn, str_tmp_top);
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
            }
            if (top(oper_stack, cur_oper_stack) == '(') {
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
            }
            if ((top(oper_stack, cur_oper_stack) >= 'A' && top(oper_stack, cur_oper_stack) <= 'Z')
                || (top(oper_stack, cur_oper_stack) >= 'a' && top(oper_stack, cur_oper_stack) <= 'z')) {
                cur_rpn = push(rpn, cur_rpn, oper_stack[cur_oper_stack-1]);
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
            }
        }
    }
    while (!is_empty(oper_stack, cur_oper_stack)) {
        char top_oper[MAX_ELEMENT_SIZE];
        top_oper[0] = top(oper_stack, cur_oper_stack);
        cur_rpn = push(rpn, cur_rpn, top_oper);
        cur_oper_stack = pop(oper_stack, cur_oper_stack);
    }

    printf("{");
    for (int i = 0; i < cur_rpn; i++) {
        printf("%s,",rpn[i]);
    }
    printf("}\n");

    int rpn_objects_counter = 0;
    for (int i = 0; rpn[i][0] != '\0'; i++) {
        rpn_objects_counter++;
    }

    // заменяем в rpn переменные на соответствующие значения
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
