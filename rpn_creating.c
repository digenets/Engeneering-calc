#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "pow_utilities.h"
#include "rpn_creating.h"
#include "constants.h"
#include "stack_strings.h"
#include "calculation.h"

char* ReplaceUnaryMinus(char* expression) {
    // либо минус стоит в начале выражения: -1+...
    // либо в середине в скобках: ... + (-1) + ...
    char* replaced = (char*) malloc(sizeof(char) * MAX_EXPR_SIZE);
    strcpy(replaced, expression);
    if (expression[0] == '-') {
        replaced[0] = '~';
    }
    for (int i = 1; i < strlen(expression); ++i) {
        if (expression[i-1] == '(' && expression[i] == '-') {
            replaced[i] = '~';
        }
    }
    return replaced;
}

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

double pow_counting(char * expression, int* position, LINEAR_MAP* vars_map){
    *position+=4;
    int exp_rpn_objects_counter = 0;
    int c = 0;
    char* tmp_exp = (char*) malloc(sizeof(char) * MAX_EXPR_SIZE);
    memset(tmp_exp, '\0', MAX_EXPR_SIZE);
    char** exp1_rpn = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE);
    for (int j = 0; j < MAX_RPN_SIZE; ++j) {
        exp1_rpn[j] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(exp1_rpn[j], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }
    char** exp2_rpn = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE);
    for (int j = 0; j < MAX_RPN_SIZE; ++j) {
        exp2_rpn[j] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(exp2_rpn[j], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }

    while (expression[*position] != ','){
        *tmp_exp = expression[*position];
        ++*position;
        tmp_exp++;
        c++;
    }
    tmp_exp -= c;
    ++*position;
    if (expression[*position] == ' ')
        ++*position;
    exp1_rpn = GetRpn(tmp_exp, &exp_rpn_objects_counter, vars_map);
    ReplaceWithVarsValues(exp1_rpn, exp_rpn_objects_counter, vars_map);
    double exp1_result = Calculate(exp1_rpn, exp_rpn_objects_counter);
    memset(tmp_exp, '\0', MAX_ELEMENT_SIZE);
    int exp_symb_number = 0;
    c = 0;
    while (expression[*position] != ')'){
        exp_symb_number++;
        if (expression[*position] == '('){
            while(expression[*position] != ')'){
                ++*position;
                c++;
                exp_symb_number++;
            }
            if (expression[*position] != ')') {
                ++*position;
                c++;
                exp_symb_number++;
            }
        }
        ++*position;
        c++;
    }
    *position -= c;
    int tmp = c;
    c = 0;
    for (int j = *position; j < *position + exp_symb_number; j++){
        *tmp_exp = expression[j];
        c++;
        tmp_exp++;
    }
    tmp_exp-=c;
    exp_rpn_objects_counter = 0;
    exp2_rpn = GetRpn(tmp_exp, &exp_rpn_objects_counter, vars_map);
    ReplaceWithVarsValues(exp2_rpn, exp_rpn_objects_counter, vars_map);
    double exp2_result = Calculate(exp2_rpn, exp_rpn_objects_counter);
    double pow_result = pow(exp1_result, exp2_result);
    *position += tmp+1;
    free(tmp_exp);
    free(exp1_rpn);
    free(exp2_rpn);
    return pow_result;
}
      /*  else{
            position-=3;
        } */


char** GetRpn(char* expression, int* rpn_objects_counter, LINEAR_MAP* vars_map) {
    expression = ReplaceUnaryMinus(expression);  // убирается унарный минус

    int cur_rpn = 0;
    char** rpn = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE); // массив строк под rpn
    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        rpn[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(rpn[i], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }
    int cur_oper_stack = 0;
    char** oper_stack = (char**) malloc(sizeof(char*) * MAX_RPN_SIZE); // массив строк под операторный стек
    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        oper_stack[i] = (char*) malloc(sizeof(char) * MAX_ELEMENT_SIZE); // выделяется память под строки
        memset(oper_stack[i], '\0', MAX_ELEMENT_SIZE); // строки заполняются '\0'
    }


    for (int i = 0; i < strlen(expression); i++) {
        if (i>strlen(expression))
            break;
        int is_func = 0;
        int is_variable = 0;
        int k = 0;
        double a = 0;
        char token[MAX_ELEMENT_SIZE] = { '\0' }; // variable, function name or a number
        char tmp_func_str[MAX_ELEMENT_SIZE] = { '\0' };
        /* if (is_pow(expression, i)){
            a = pow_counting(expression, &i, vars_map);
        } */
        if (((expression[i] >= '0' && expression[i] <= '9')
            || (expression[i] >= 'A' && expression[i] <= 'Z')
            || (expression[i] >= 'a' && expression[i] <= 'z')) && !is_pow(expression, i)) { // not an operand
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

        if ((!is_func && is_variable) || is_pow(expression, i)) {
            if (is_pow(expression, i)){
                char* pow_string;
                pow_string = malloc(sizeof(char) * MAX_EXPR_SIZE);
                memset(pow_string, '\0', MAX_EXPR_SIZE);
                // ftoa(pow_counting(expression, &i, vars_map), pow_string, 3);
                double d = pow_counting(expression, &i, vars_map);
                sprintf(pow_string, "%f", d);
                push(rpn, cur_rpn, pow_string);
            }
            else {
                cur_rpn = push(rpn, cur_rpn, token); // function "push" return cur_rpn++
                i = i + k - 1;
            }
        }
        else if (is_func) {
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
        }                                                                         // если функция
        else if (is_oper(expression[i])) {
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
        }                                                          // если оператор
        else if (expression[i] == '(') {
            char open_bracket_str[MAX_ELEMENT_SIZE] = {'\0'};
            open_bracket_str[0] = '('; //str where first element is "("
            cur_oper_stack = push(oper_stack, cur_oper_stack, open_bracket_str);
        }                                                            // если открывающаяся скобка
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
        }                                                            // если закрывающаяся скобка
    }
    while (!is_empty(oper_stack, cur_oper_stack)) {
        char top_oper[MAX_ELEMENT_SIZE];
        top_oper[0] = top(oper_stack, cur_oper_stack);
        cur_rpn = push(rpn, cur_rpn, top_oper);
        cur_oper_stack = pop(oper_stack, cur_oper_stack);
    }

    for (int i = 0; i < MAX_RPN_SIZE; ++i) {
        free(oper_stack[i]);
    }
    free(oper_stack);

    printf("{");
    for (int i = 0; i < cur_rpn; i++) {
        printf("%s,",rpn[i]);
    }
    printf("}\n");

    for (int i = 0; rpn[i][0] != '\0'; i++) {
        ++(*rpn_objects_counter);
    }
    return rpn;
}
