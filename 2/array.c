#include"array.h"

typedef struct Array
{
    int size;
    ARRAY_TYPE data[ARRAY_MAX_SIZE];
}Array;

void array_init(Array* arr)
{
    arr->size=ARRAY_MAX_SIZE;
    memset(array,0,sizeof(array));
    return;
}

ARRAY_TYPE* array_ptr_at(Array* arr)
{
    return arr->data;
}

bool array_empty(Array* arr)
{
    if(arr->size<=0)
        return true;
    return false;
}

int array_size(Array* arr)
{
    return arr->size;
}

int array_max_size(Array* arr)
{
    return arr->size;
}
