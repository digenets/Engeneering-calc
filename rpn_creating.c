#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "rpn_creating.h"
#include "constants.h"
#include "stack_strings.h"
#include "calculation.h"

char* ReplaceUnaryMinus(char* expression) {
    // либо минус стоит в начале выражения: -1+...
    // либо в середине в скобках: ... + (-1) + ...
    char* result = (char*) malloc(sizeof(char) * MAX_EXPR_SIZE);
    strcpy(result, expression);
    if (expression[0] == '-') {
        result[0] = '~';
    }
    for (int i = 1; i < strlen(expression); ++i) {
        if (expression[i-1] == '(' && expression[i] == '-') {
            result[i] = '~';
        }
    }
    return result;
}

int is_operator (char symb) {
    if (symb == '^' || symb == '~') {
        return 3;
    }
    if ((symb == '*') || (symb == '/')) {
        return 2;
    }
    if ((symb == '+') || (symb == '-')) {
        return 1;
    }
    return 0;
}

bool is_pow(char* expression, int position) {
    if (expression[position] == 'p') {
        char token[MAX_EXPR_SIZE] = {'\0'};
        for (int j = 0; j < 4; j++) {
            token[j] = expression[position];
            position++;
        }
        if (strcmp(token, "pow(") == 0)
            return true;
        else return false;
    }
    return false;
}
// pow(adc+5, 2)
double pow_counting(char * expression, int* position, LINEAR_MAP* vars_map){
    int exp_rpn_objects_number = 0;
    int exp1_symb_number = 0;
    int c = 0;

    *position+=4;

    char* exp_buffer = (char*) malloc(sizeof(char) * MAX_EXPR_SIZE);
    memset(exp_buffer, '\0', MAX_EXPR_SIZE);

    char** exp1_rpn = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE);
    for (int j = 0; j < MAX_RPN_SIZE; ++j){
        exp1_rpn[j] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(exp1_rpn[j], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }

    char** exp2_rpn = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE);
    for (int j = 0; j < MAX_RPN_SIZE; ++j){
        exp2_rpn[j] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(exp2_rpn[j], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }

    while (expression[*position] != ',') {
        *exp_buffer = expression[*position];
        ++*position;
        exp_buffer++;
        c++;
    }
    exp_buffer -= c;
    ++*position;

    if (expression[*position] == ' ')
        ++*position;

    exp1_rpn = GetRpn(exp_buffer, &exp_rpn_objects_number, vars_map);
    ReplaceWithVarsValues(exp1_rpn, exp_rpn_objects_number, vars_map);
    double exp1_result = Calculate(exp1_rpn, exp_rpn_objects_number);
    memset(exp_buffer, '\0', MAX_ELEMENT_SIZE);
    c = 0;

    while (expression[*position] != ')'){
        exp1_symb_number++;
        if (expression[*position] == '('){
            while(expression[*position] != ')'){
                ++*position;
                c++;
                exp1_symb_number++;
            }
            if (expression[*position] != ')') {
                ++*position;
                c++;
                exp1_symb_number++;
            }
        }
        ++*position;
        c++;
    }
    *position -= c;
    int tmp = c;
    c = 0;

    for (int j = *position; j < *position + exp1_symb_number; j++){
        *exp_buffer = expression[j];
        c++;
        exp_buffer++;
    }

    exp_buffer-=c;
    exp_rpn_objects_number = 0;
    exp2_rpn = GetRpn(exp_buffer, &exp_rpn_objects_number, vars_map);
    ReplaceWithVarsValues(exp2_rpn, exp_rpn_objects_number, vars_map);
    double exp2_result = Calculate(exp2_rpn, exp_rpn_objects_number);
    double pow_result = pow(exp1_result, exp2_result);
    *position += tmp;
    free(exp_buffer);
    free(exp1_rpn);
    free(exp2_rpn);
    return pow_result;
}

char** GetRpn(char* expression, int* rpn_objects_number, LINEAR_MAP* vars_map) {
    expression = ReplaceUnaryMinus(expression);  // убирается унарный минус

    int rpn_position = 0;
    char** rpn = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE); // массив строк под rpn
    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        rpn[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(rpn[i], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }
    int operator_stack_position = 0;
    char** operator_stack = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE); // массив строк под операторный стек
    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        operator_stack[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(operator_stack[i], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }


    for (int i = 0; i < strlen(expression); i++) {
        
        int      is_function = 0;
        int      is_variable = 0;
        int      k = 0;
        char     token[MAX_ELEMENT_SIZE] = { '\0' };
        char     function_buffer[MAX_ELEMENT_SIZE] = {'\0' };
 
        if (((expression[i] >= '0' && expression[i] <= '9')
            || (expression[i] >= 'A' && expression[i] <= 'Z')
            || (expression[i] >= 'a' && expression[i] <= 'z')) && !is_pow(expression, i)) {
            for (int j = i; j < strlen(expression); j++) {
                if (is_operator(expression[j])) {
                    break;
                }
                if (expression[j] == ')') {
                    break;
                }
                token[k] = expression[j];
                k++;
            }
            if (token[strlen(token) - 1] == ' ') {
                token[strlen(token) - 1] = '\0';
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
            } else {
                strcpy(function_buffer, token);
            }
        }

            if (is_pow(expression, i)){
                char* pow_string = malloc(sizeof(char) * MAX_EXPR_SIZE);
                memset(pow_string, '\0', MAX_EXPR_SIZE);
                double pow_counting_result = pow_counting(expression, &i, vars_map);
                sprintf(pow_string, "%lf", pow_counting_result);
                rpn_position = push(rpn, rpn_position, pow_string);
            }
            else if ((!is_function && is_variable)) {
                rpn_position = push(rpn, rpn_position, token); // function "push" return rpn_position++
                i = i + k - 1;
            }

        else if (is_function) {
            int counter = 0;
            for (int j = 0; j < strlen(function_buffer); j++) {
                counter++;
                if (function_buffer[j] == '(') {
                    break;
                }
            }
            i = i + counter - 2;
            for (int j = 0; j < strlen(function_buffer); j++) {
                if (j >= counter - 1) {
                    function_buffer[j] = '\0';
                }
            }
            operator_stack_position = push(operator_stack, operator_stack_position, function_buffer);
        }
        else if (is_operator(expression[i])) {
            char head = top(operator_stack, operator_stack_position);
            char str_head[MAX_ELEMENT_SIZE] = {'\0'};
            str_head[0] = head;
            while  ((!is_empty(operator_stack, operator_stack_position) && is_operator(str_head[0])) // пока стек не пустой и в топе оператор
                    && (((is_operator(expression[i]) == 1 || is_operator(expression[i]) == 2) && (is_operator(expression[i]) <=
                    is_operator(str_head[0]))/*и приоритет токена меньше либо равен топу*/) || ((is_operator(expression[i]) == 3)
                    /*токен правоасоц*/ && (is_operator(expression[i]) < is_operator(str_head[0])) /*и приоритет токена меньше топа*/))
                    && str_head[0] != '(')
            {
                head = top(operator_stack, operator_stack_position);
                str_head[0] = head;
                rpn_position = push(rpn, rpn_position, str_head);
                operator_stack_position = pop(operator_stack, operator_stack_position);

                head = top(operator_stack, operator_stack_position);
                memset(str_head, '\0', strlen(str_head));
                str_head[0] = head;
            }


            char str_oper[MAX_ELEMENT_SIZE] = {'\0' };
            str_oper[0] = expression[i];
            operator_stack_position = push(operator_stack, operator_stack_position, str_oper);
        }
        else if (expression[i] == '(') {
            char open_bracket_str[MAX_ELEMENT_SIZE] = {'\0'};
            open_bracket_str[0] = '(';
            operator_stack_position = push(operator_stack, operator_stack_position, open_bracket_str);
        }
        else if (expression[i] == ')') {
            while (top(operator_stack, operator_stack_position) != '(') {
                char str_tmp_top[MAX_ELEMENT_SIZE] = {'\0'};
                char tmp_top = top(operator_stack, operator_stack_position);
                str_tmp_top[0] = tmp_top;
                rpn_position = push(rpn, rpn_position, str_tmp_top);
                operator_stack_position = pop(operator_stack, operator_stack_position);
            }
            if (top(operator_stack, operator_stack_position) == '(') {
                operator_stack_position = pop(operator_stack, operator_stack_position);
            }
            if ((top(operator_stack, operator_stack_position) >= 'A' && top(operator_stack, operator_stack_position) <= 'Z')
                || (top(operator_stack, operator_stack_position) >= 'a' && top(operator_stack, operator_stack_position) <= 'z')) {
                rpn_position = push(rpn, rpn_position, operator_stack[operator_stack_position - 1]);
                operator_stack_position = pop(operator_stack, operator_stack_position);
            }
        }
    }
    while (!is_empty(operator_stack, operator_stack_position)) {
        char head_oper[MAX_ELEMENT_SIZE];
        head_oper[0] = top(operator_stack, operator_stack_position);
        rpn_position = push(rpn, rpn_position, head_oper);
        operator_stack_position = pop(operator_stack, operator_stack_position);
    }

    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        free(operator_stack[i]);
    }
    free(operator_stack);

    printf("{");
    for (int i = 0; i < rpn_position; i++) {
        printf("%s,",rpn[i]);
    }
    printf("}\n");

    for (int i = 0; rpn[i][0] != '\0'; i++) {
        ++(*rpn_objects_number);
    }
    return rpn;
}
