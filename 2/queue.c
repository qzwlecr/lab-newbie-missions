#include"queue.h"
#include"list.h"
#include"list.c"

typedef struct Queue
{
    List data;
}Queue;

void queue_init(Queue* que)
{
    list_init(&que->data);
    return;
}

bool queue_empty(Queue* que)
{
    return list_empty(&que->data);
}

int queue_size(Queue* que)
{
    return list_size(&que->data);
}

void queue_clear(Queue* que)
{
    list_clear(&que->data);
    return;
}

void queue_push(Queue* que,QUEUE_TYPE value)
{
    list_push_back(&que->data,value);
    return;
}

void queue_pop(Queue* que)
{
    list_pop_front(&que->data);
    return;
}

QUEUE_TYPE queue_top(Queue* que)
{
    return list_begin(&que->data).data->data;
}
