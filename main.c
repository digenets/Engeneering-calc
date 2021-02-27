#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "test.h"
#include "pair_string_double.h"
#include "linear_map.h"
#include "calculation.h"

#define MAX_EXPRESSION_SIZE 100
#define MAX_VAR_NAME_SIZE 50
#define SIZE1 128
#define SIZE2 256
#define MAX_SIZE 1024

int is_oper (char c) {
    if (c == '^') {
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

void input(int *expressions_count, int *vars_count, char expressions[SIZE1][SIZE2], char vars[SIZE1][SIZE2],
           char vars_names[SIZE1][SIZE2], char c_vars[SIZE1][SIZE2], char c_vars_names[SIZE1][SIZE2], int *c_vars_count) {
    FILE *fin;
    fin = fopen("input.txt", "r");
    char file_read_str[SIZE2];
    while (fgets(file_read_str, SIZE2, fin) != NULL) {
        int len = (int)strlen(file_read_str) - 1;
        int is_varuable = 0;
        for (int i = 0; i < len; i++) {
            if (file_read_str[i] == '=') {
                is_varuable = 1;
                break;
            }
        }
        if (is_varuable == 1) {
            char vars_without_spaces[SIZE2] = {'\0'};
            int is_complex = 0;
            for (int i = 0; i < len; i++) {
                if (file_read_str[i] == 'j') {
                    is_complex = 1;
                    break;
                }
            }
            if (is_complex == 1) {
                int new_i = 0;
                for (int i = 0; i < len; i++) {
                    if (file_read_str[i] != ' ') {
                        vars_without_spaces[new_i] = file_read_str[i];
                        new_i++;
                    }
                }
                new_i = 0;
                int was_equal = 0;
                for (int i = 0; i < strlen(vars_without_spaces); i++) {
                    if (vars_without_spaces[i] == '=') {
                        i++;
                        new_i = 0;
                        was_equal = 1;
                    }
                    if (was_equal == 0) {
                        c_vars_names[*c_vars_count][i] = vars_without_spaces[i];
                    } else {
                        c_vars[*c_vars_count][new_i] = vars_without_spaces[i];
                        new_i++;
                    }
                }
                *c_vars_count = *c_vars_count + 1;
            } else {
                int new_i = 0;
                for (int i = 0; i < len; i++) {
                    if (file_read_str[i] != ' ') {
                        vars_without_spaces[new_i] = file_read_str[i];
                        new_i++;
                    }
                }
                new_i = 0;
                int was_equal = 0;
                for (int i = 0; i < strlen(vars_without_spaces) - 1; i++) {
                    if (vars_without_spaces[i] == '=') {
                        i++;
                        new_i = 0;
                        was_equal = 1;
                    }
                    if (was_equal == 0) {
                        vars_names[*vars_count][i] = vars_without_spaces[i];
                    } else {
                        vars[*vars_count][new_i] = vars_without_spaces[i];
                        new_i++;
                    }
                }
                *vars_count = *vars_count + 1;
            }
        } else if (file_read_str[0] != '\0' && file_read_str[0] != '\n'){
            char expressions_without_spaces[SIZE2] = {'\0'};
            int new_i = 0;
            for (int i = 0; i < len; i++) {
                if (file_read_str[i] != ' ' && file_read_str[i] != '\n') {
                    expressions_without_spaces[new_i] = file_read_str[i];
                    new_i++;
                }
            }
            new_i = 0;
            strcpy(expressions[*expressions_count], expressions_without_spaces);
            *expressions_count = *expressions_count + 1;
        }
    }
    fclose(fin);
}

void output(int *expressions_count, int *vars_count, char expressions[SIZE1][SIZE2], char vars[SIZE1][SIZE2],
            char vars_names[SIZE1][SIZE2], int expressions_results[SIZE1], char c_vars[SIZE1][SIZE2],
            char c_vars_names[SIZE1][SIZE2], int *c_vars_count) {
    FILE *fout;
    fout = fopen("output.txt", "w");
    fprintf(fout, "vars:\n");
    for (int i = 0; i < *vars_count; i++) {
        fprintf(fout, "%s = %s\n", vars_names[i], vars[i]);
    }
    fprintf(fout, "\ncomplex vars:\n");
    for (int i = 0; i < *c_vars_count; i++) {
        fprintf(fout, "%s = %s\n", c_vars_names[i], c_vars[i]);
    }
    fprintf(fout, "\nexpressions:\n");
    for (int i = 0; i < *expressions_count; i++) {
        fprintf(fout, "%s = /*%d*/(result of calculation)\n", expressions[i], expressions_results[i]);
    }
    fclose(fout);
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
    char c_vars[SIZE1][SIZE2] = {'\0'};
    char c_vars_names[SIZE1][SIZE2] = {'\0'};
    int c_vars_count = 0;
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
    int cur_rpn = 0;
    char rpn[MAX_SIZE][256];
    int cur_oper_stack = 0;
    char oper_stack[MAX_SIZE][256];

    if (argc > 3 && strcmp(argv[3], TEST) == 0) {
        Test();
        return 0;
    }

    FILE* file_input = fopen(argv[1], "r");

    int number_of_lines;
    fscanf(file_input, "%d\n", &number_of_lines);

    char* expression = (char*) malloc(sizeof(char) * MAX_EXPRESSION_SIZE);
    fgets(expression, MAX_EXPRESSION_SIZE, file_input);
    if (expression[strlen(expression) - 1] == '\n') {
        expression[strlen(expression) - 1] = '\0';
    }

    int number_of_numbers = number_of_lines - 1;
    char** input_strings = (char**) malloc(sizeof(char*) * number_of_numbers);
    for (int i = 0; i < number_of_numbers; ++i) {
        input_strings[i] = (char*) malloc(sizeof(char) * MAX_EXPRESSION_SIZE);
        fgets(input_strings[i], MAX_EXPRESSION_SIZE, file_input);
    }
    LINEAR_MAP vars_map = ParseVariables(input_strings, number_of_numbers);

    for (int i = 0; i < strlen(expression); i++) {
        int is_func = 0;
        int is_variable = 0;
        int k = 0;
        char tmp_str[256] = { '\0' };
        char tmp_func_str[256] = { '\0' };
        if ((expression[i] >= '0' && expression[i] <= '9')
                || (expression[i] >= 'A' && expression[i] <= 'Z')
                || (expression[i] >= 'a' && expression[i] <= 'z')) {
            for (int j = i; j < strlen(expression); j++) {
                if (is_oper(expression[j])) {
                    break;
                }
                if (expression[j] == ')') {//WARNING
                    break;
                }
                tmp_str[k] = expression[j];
                k++;
            }

            // если имя переменной заканчивается пробелом, убираем его
            if (tmp_str[strlen(tmp_str) - 1] == ' ') {
                tmp_str[strlen(tmp_str) - 1] = '\0';
            }

            for (int n = 0; n < strlen(tmp_str); n++) {
                if (tmp_str[n] == '(') {
                    is_func = 1;
                    is_variable = 0;
                    break;
                }
            }
            if (!is_func) {
                is_func = 0;
                is_variable = 1;
            }
            else {
                strcpy(tmp_func_str, tmp_str);
            }
        }
        if (!is_func && is_variable) {
            cur_rpn = push(rpn, cur_rpn, tmp_str); // function "push" return cur_rpn++
            i = i + k - 1;
        }
        else if (is_func) {
            // expon(23424+3423+2)
            //count number of letters in name of function
            int counter = 0;
            for (int j = 0; j < strlen(tmp_func_str); j++) {
                if (tmp_func_str[j] == '(') {
                    counter++;
                    break;
                }
                counter++;
            }
            i = i + counter - 2;
            for (int j = 0; j < strlen(tmp_func_str); j++) {
                if (j < counter - 1) {
                    tmp_func_str[j] = tmp_func_str[j];
                } else {
                    tmp_func_str[j] = '\0';
                }
            }
            cur_oper_stack = push(oper_stack, cur_oper_stack, tmp_func_str);
        }
        else if (is_oper(expression[i])) {
            char tmp_top = top(oper_stack, cur_oper_stack);
            char str_tmp_top[256] = {'\0'};
            str_tmp_top[0] = tmp_top;
            // Check that top of stack is operator
            while  ( (top(oper_stack, cur_oper_stack) && is_oper(str_tmp_top[0])) // пока стек не пустой и в топе оператор
                     &&

                     (
                             /*либо*/((is_oper(expression[i]) == 1 || is_oper(expression[i]) == 2)/*токен левоасоц*/ && (is_oper(expression[i]) <= is_oper(str_tmp_top[0]))/*и приоритет токена меньше либо равен топу*/)
                                     ||
                                     /*либо*/((is_oper(expression[i]) == 3) /*токен правоасоц*/ && (is_oper(expression[i]) < is_oper(str_tmp_top[0])) /*и приоритет токена меньше топа*/)
                     )

//                     && is_oper(str_tmp_top[0]) != 40
                    && str_tmp_top[0] != '('
                    )
            {//while cicle
                tmp_top = top(oper_stack, cur_oper_stack);
                str_tmp_top[0] = tmp_top;
                cur_rpn = push(rpn, cur_rpn, str_tmp_top);
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
                //retake
                tmp_top = top(oper_stack, cur_oper_stack);
                for (int j = 0; j < 256; j++) {
                    str_tmp_top[j] = '\0';
                }
                str_tmp_top[0] = tmp_top;
            }
            //end of while cicle

            char str_tmp_oper[256] = { '\0' };
            str_tmp_oper[0] = expression[i];
            cur_oper_stack = push(oper_stack, cur_oper_stack, str_tmp_oper);
        }
        else if (expression[i] == '(') {
            char open_braket_str[256] = {'\0'};
            open_braket_str[0] = '('; //str where first element is "("
            cur_oper_stack = push(oper_stack, cur_oper_stack, open_braket_str);
        }
        else if (expression[i] == ')') {
            while (top(oper_stack, cur_oper_stack) != '(') {
                char str_tmp_top[256] = {'\0'};
                char tmp_top = top(oper_stack, cur_oper_stack);
                str_tmp_top[0] = tmp_top;
                cur_rpn = push(rpn, cur_rpn, str_tmp_top);
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
            }
            if (top(oper_stack, cur_oper_stack) == '(') {
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
            }
            if ((top(oper_stack, cur_oper_stack) >= 'A' && top(oper_stack, cur_oper_stack) <= 'Z') || (top(oper_stack, cur_oper_stack) >= 'a' && top(oper_stack, cur_oper_stack) <= 'z')) {
                cur_rpn = push(rpn, cur_rpn, oper_stack[cur_oper_stack-1]);
                cur_oper_stack = pop(oper_stack, cur_oper_stack);
            }
        }
    }
    while (!is_empty(oper_stack, cur_oper_stack)) {
        char top_oper[256];
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
