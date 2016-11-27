#include"vector.h"
#include<string.h>

typedef struct Vector
{
    int capacity;
    int size;
    VECTOR_TYPE* data;
}Vector;

void vector_init(Vector* vec)
{
    vec->data=(VECTOR_TYPE*)malloc(sizeof(VECTOR_TYPE));
    vec->size=0;
    vec->capacity=1;
    return;
}

void vector_clear(Vector* vec)
{
    memset(vec->data,0,vec->size*sizeof(VECTOR_TYPE));
    vec->size=0;
    vec->capacity=1;
    return;
}

VECTOR_TYPE* vector_ptr_at(Vector* vec)
{
    return vec->data;
}

bool vector_empty(Vector* vec)
{
    return (vec->size==0);
}

int vector_size(Vector* vec)
{
    return vec->size;
}

int vector_capacity(Vector* vec)
{
    return vec->capacity;
}

void vector_new_capacity(Vector* vec,int new_cap)
{
    VECTOR_TYPE* temp=(VECTOR_TYPE*)malloc(sizeof(VECTOR_TYPE)*new_cap);
    memcpy(temp,vec->data,sizeof(VECTOR_TYPE)*vec->size);
    free(vec->data);
    vec->capacity=new_cap;
    vec->data=temp;
    return;
}

void vector_reserve(Vector* vec,int new_cap)
{
    if(new_cap>vec->capacity)
        vector_new_capacity(vec,new_cap);
    return;
}

void vector_resize(Vector* vec,int new_size)
{
    if(vec->size>new_size)
    {
        vec->size=new_size;
    }
    else
    {
        if(new_size<vec->capacity)
        {
            memset(vec->data+vec->size,0,(sizeof(VECTOR_TYPE))*(new_size-vec->size));
            vec->size=new_size;
        }
        else
        {
            vector_new_capacity(vec,new_size);
            memset(vec->data+vec->size,0,sizeof(VECTOR_TYPE)*(new_size-vec->size));
            vec->size=new_size;
        }
    }
    return;
}

void vector_shrink_to_fit(Vector* vec)
{
    vec->capacity=vec->size;
    return;
}

void vector_insert(Vector* vec,int where,VECTOR_TYPE value)
{
    if(where<=vec->size)
    {
        if(vec->size==vec->capacity)
        {
            vector_new_capacity(vec,vec->size*2);
            memmove(vec->data+where+1,vec->data+where,(vec->size-where)*sizeof(VECTOR_TYPE));
        }
        ++vec->size;
        vec->data[where]=value;
    }
    return;
}

void vector_erase(Vector* vec,int where)
{
    if(where<vec->size&&where>=0&&vec->size>0)
    {
        memmove(vec->data+where,vec->data+where+1,(vec->size-where)*sizeof(VECTOR_TYPE));
        --vec->size;
    }
}

void vector_push_back(Vector* vec,VECTOR_TYPE value)
{
    if(vec->size==vec->capacity)
    {
        vector_new_capacity(vec,vec->size*2);
    }
    vec->data[vec->size++]=value;
    return;
}

void vector_pop_back(Vector* vec)
{
    vec->data[vec->size--]=0;
    return;
}

