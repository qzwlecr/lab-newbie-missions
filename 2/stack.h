#include <stdbool.h>
#include <stdlib.h>
typedef double STACK_TYPE;
// TIPS: re-define the above identifiers
// in *.cpp file to make them shorter
// e.g. typedef T = STACK_TYPE;
// NOTE YOU CAN ONLY DO THAT IN *.cpp FILES

typedef struct Stack Stack;

// functions
// note the absence of reference
void stack_init(Stack* stk);
bool stack_empty(Stack* stk);
int stack_size(Stack* stk);

void stack_clear(Stack* stk);

void stack_push(Stack* stk, STACK_TYPE value);
void stack_pop(Stack* stk);
// return pointer to make it mutable is meaningless.
STACK_TYPE stack_top(Stack* stk);

