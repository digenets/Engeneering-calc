#include "rpn_creating.h"
#include "constants.h"
#include <string.h>
#include <stdlib.h>

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
