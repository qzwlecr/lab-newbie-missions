#include"priority_queue.h"

#include"vector.c"

#define HEAP_PARENT(x) ((x-1)>>1)
#define HEAP_LEFT_CHILD(x) ((x<<1)+1)
#define HEAP_RIGHT_CHILD(x) ((x<<1)+2)

typedef struct Priority_queue
{
    Vector data;
}Priority_queue;

void priority_queue_init(Priority_queue* stk)
{
    vector_init(&stk->data);
    return;
}

bool priority_queue_empty(Priority_queue* stk)
{
    return vector_empty(&stk->data);
}

int priority_queue_size(Priority_queue* stk)
{

    return vector_size(&stk->data);
}

void priority_queue_clear(Priority_queue* stk)
{
    vector_clear(&stk->data);
    return;
}

void priority_queue_push(Priority_queue* stk,PRIORITY_QUEUE_TYPE value)
{
    VECTOR_TYPE* data=vector_ptr_at(&stk->data);
    vector_push_back(&stk->data,0);
    int node=vector_size(&stk->data),parent=HEAP_PARENT(node);
    while(data[parent]<value)
    {
        data[node]=data[parent];
        node=parent;
        parent=HEAP_PARENT(node);
    }
    data[node]=value;
    return;
}

void priority_queue_pop(Priority_queue* stk)
{
    VECTOR_TYPE* data=vector_ptr_at(&stk->data);
    PRIORITY_QUEUE_TYPE last_element=data[vector_size(&stk->data)-1];
    vector_pop_back(&stk->data);
    int parent=0,child=0,size=vector_size(&stk->data);
    while(HEAP_LEFT_CHILD(parent)<size)
    {
        child=HEAP_LEFT_CHILD(parent);
        if(child!=size-1&&data[child]<data[child+1])
            ++child;
        if(last_element<data[child])
            data[parent]=data[child];
        else
            break;
        parent=child;
    }
    data[parent]=last_element;
    return;
}

PRIORITY_QUEUE_TYPE Priority_queue_top(Priority_queue* stk)
{
    return vector_ptr_at(&stk->data)[0];
}
