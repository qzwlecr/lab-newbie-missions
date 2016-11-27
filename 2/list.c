#include"list.h"

typedef struct Node
{
    LIST_TYPE data;
    struct Node *next;
    struct Node *last;
}Node;

typedef struct List
{
    Node *head;
    int size;
}List;

typedef struct ListIterator
{
    Node *data;
}ListIterator;

bool list_empty(List* lis)
{
    return (lis->size==0);
}

int list_size(List* lis)
{
    return lis->size;
}

static Node* list_create_newnode(LIST_TYPE data)
{
    Node *node=(Node*)malloc(sizeof(Node));
    if(node==NULL)
        return NULL;
    node->data=data;
    return node;

}

void list_init(List* lis)
{
    Node *node=(Node*)malloc(sizeof(Node));
    if(node==NULL)
    {
        free(lis);
        lis=NULL;
        exit(1);
    }
    node->data=0;
    node->next=node;
    node->last=node;
    lis->head=node;
    lis->size=0;
    return;
}

void list_clear(List* lis)
{
    Node *node=lis->head->next;
    Node *end=lis->head;
    Node *tmp=NULL;
    while(node!=end)
    {
        tmp=node->next;
        free(node);
        node=tmp;
    }
    free(end);
    free(lis);
    lis=NULL;
    return;
}

static void list_node_link_to_list(List *lis,Node *node,Node *next_node)
{
    Node *last_node=next_node->last;
    last_node->next=node;
    node->next=next_node;
    next_node->last=node;
    node->last=last_node;
    ++lis->size;
    return;
}

void list_insert(List* lis, ListIterator where, LIST_TYPE value)
{
    Node *node=list_create_newnode(value);
    if(node==NULL)
    {
        return;
    }
    node->next=NULL;
    node->last=NULL;
    list_node_link_to_list(lis,node,where.data);
    ++lis->size;
    return;
}

void list_erase(List* lis, ListIterator where)
{
    Node *next_node=where.data->next;
    Node *last_node=where.data->last;;
    next_node->last=last_node;
    last_node->next=next_node;
    --lis->size;
    if(where.data!=lis->head)
        free(where.data);
    return;
}

void list_push_back(List* lis, LIST_TYPE value)
{
    Node *node=list_create_newnode(value);
    list_node_link_to_list(lis,node,lis->head);
    return;
}

void list_pop_back(List* lis)
{
    Node *node=lis->head->last;
    list_erase(lis,(ListIterator){node});
    return;
}

void list_push_front(List* lis, LIST_TYPE value)
{
    Node *node=list_create_newnode(value);
    list_node_link_to_list(lis,node,lis->head->next);
    return;
}

void list_pop_front(List* lis)
{
    Node *node=lis->head->next;
    ListIterator new_iterator;
    new_iterator.data=node;
    list_erase(lis,(ListIterator){node});
    return;
}

void ListIterator_self_increase(ListIterator* pIter)
{
    pIter->data=pIter->data->next;
    return;
}

void ListIterator_self_decrease(ListIterator* pIter)
{
    pIter->data=pIter->data->last;
    return;
}

ListIterator list_begin(List* lis)
{
    return (ListIterator){lis->head->next};
}

ListIterator list_end(List* lis)
{
    return (ListIterator){lis->head};
}

bool ListIterator_equal(ListIterator iter_a, ListIterator iter_b)
{
    return (iter_a.data->data==iter_b.data->data);
}

bool ListIterator_not_equal(ListIterator iter_a, ListIterator iter_b)
{
    return (iter_a.data->data!=iter_b.data->data);
}

LIST_TYPE* list_get_data_ptr(ListIterator* iter)
{
    return &(iter->data->data);
}
