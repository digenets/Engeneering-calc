#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    char vars[SIZE1][SIZE2] = {'\0'};
    char vars_names[SIZE1][SIZE2] = {'\0'};
    int vars_count = 0;
    char funcs[13][SIZE2] = {'\0'};
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
    int cur_rpn = 0;
    char rpn[MAX_SIZE][256];
    int cur_oper_stack = 0;
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
            int is_func = 0;
            int is_number = 0;
            int k = 0;
            char tmp_str[256] = { '\0' };
            char tmp_func_str[256] = { '\0' };
            if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')) {
                for (int j = i; j < strlen(str) - 1; j++) {
                    if (is_oper(str, j)) {
                        break;
                    }
                    if (str[j] == ')') {//WARNING
                        break;
                    }
                    tmp_str[k] = str[j];
                    k++;
                }
                for (int n = 0; n < strlen(tmp_str); n++) {
                    if (tmp_str[n] == '(') {
                        is_func = 1;
                        is_number = 0;
                        break;
                    }
                }
                if (!is_func) {
                    is_func = 0;
                    is_number = 1;
                }
                else {
                    strcpy(tmp_func_str, tmp_str);
                }        }
            if (!is_func && is_number) {
                cur_rpn = push(rpn, cur_rpn, tmp_str); // function "push" return cur_rpn++
                i = i + k - 1;

            }
            else if (is_func) {
                char symb_zero[1];
                symb_zero[0] = 40; //str where first element is "("
                cur_oper_stack = push(oper_stack, cur_oper_stack, symb_zero); // 40 - ASCII CODE OF "("
                // expon(23424+3423+2)
                //count number of letters in name of function
                int counter = 0;
                for (int j = 0; j < strlen(tmp_func_str); j++) {
                    if (tmp_func_str[j] == 40) {
                        counter++;
                        break;
                    }
                    counter++;
                }
                i = i + counter - 1;
                for (int j = 0; j < strlen(tmp_func_str); j++) {
                    if (j < counter - 1) {
                        tmp_func_str[j] = tmp_func_str[j];
                    } else {
                        tmp_func_str[j] = '\0';
                    }
                }
                cur_oper_stack = push(oper_stack, cur_oper_stack, tmp_func_str);
            }
            if (is_oper(str, i)) {
                char tmp_top = top(oper_stack, cur_oper_stack);
                char str_tmp_top[256] = {'\0'};
                str_tmp_top[0] = tmp_top;
                // Check that top of stack is operator
                while  ( (top(oper_stack, cur_oper_stack) && is_oper(str_tmp_top, 0)) // пока стек не пустой и в топе оператор
                         &&

                         (
                                 /*либо*/((is_oper(str, i) == 1 || is_oper(str, i) == 2)/*токен левоасоц*/ && (is_oper(str, i) <= is_oper(str_tmp_top, 0))/*и приоритет токена меньше либо равен топу*/)
                                         ||
                                         /*либо*/((is_oper(str, i) == 3) /*токен правоасоц*/ && (is_oper(str, i) < is_oper(str_tmp_top, 0)) /*и приоритет токена меньше топа*/)
                         )

                         && is_oper(str_tmp_top, 0) != 40
                        )
                {//while cicle
                    tmp_top = top(oper_stack, cur_oper_stack);
                    str_tmp_top[0] = tmp_top;
                    cur_rpn = push(rpn, cur_rpn, str_tmp_top);
                    cur_oper_stack = pop(oper_stack, cur_oper_stack);
                }
                //end of while cicle

                char str_tmp_oper[256] = { '\0' };
                str_tmp_oper[0] = str[i];
                cur_oper_stack = push(oper_stack, cur_oper_stack, str_tmp_oper);
            }
        }
        while (!is_empty(oper_stack, cur_oper_stack)) {
            char top_oper[256];
            top_oper[0] = top(oper_stack, cur_oper_stack);
            cur_rpn = push(rpn, cur_rpn, top_oper);
            cur_oper_stack = pop(oper_stack, cur_oper_stack);
        }

    return 0;
}
