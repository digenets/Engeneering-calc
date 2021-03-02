#include "stack_strings.h"
#include <string.h>

int push(char** stack, int cur, char str[MAX_ELEMENT_SIZE]) {
    if (cur < MAX_RPN_SIZE)
    {
        strcpy(stack[cur], str);
        cur++;
    }
    return cur;
}

int is_empty(char** stack, unsigned cur) {
    return !cur;
}

char top(char** stack, unsigned cur) {
    if (!is_empty(stack, cur))
        return stack[cur - 1][0];
    return 0;
}

int pop(char** stack, unsigned cur){
    if(!is_empty(stack, cur))
        --cur;
    return cur;
}