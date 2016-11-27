#include"stack.h"
#include"vector.h"

typedef struct Stack
{
    Vector data;
}Stack;

void stack_init(Stack* stk)
{
    vector_init(stk->data);
    return;
}

bool stack_empty(Stack* stk)
{
    return vector_empty(&stk->data);
}

int stack_size(Stack* stk)
{
    return vector_size(&stk->data);
}

void stack_clear(Stack* stk)
{
    vector_clear(&stk->data);
    return;
}

void stack_push(Stack* stk,STACK_TYPE value)
{
    vector_push_back(&stk->data,value);
    return;
}

void stack_pop(Stack* stk)
{
    vector_pop_back(&stk->data);
    return;
}

STACK_TYPE stack_top(Stack* stk)
{
    Vector* it=&stk->data;
    return it->data[it->size];
}
