#include"set.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define RED 0
#define BLACK 1
#define REAL 0
#define THREAD 1


typedef struct Node
{
    SET_TYPE key;
    bool color;
//    bool tag[2];//declare whether a pointer is a thread or a real pointer
    struct Node *child[2],*parent;
}Node;

static Node* Header;

typedef struct Set
{
    Node *root;
    Node *first;
    Node *last;
    int size;
}Set;

typedef struct SetIterator
{
    Node *pointer;
    Set *set;
}SetIterator;

static Node* set_create_new_node(SET_TYPE key)
{
    Node* node=(Node*)malloc(sizeof(Node));
    if(node==NULL)
    {
        printf("create error!");
        exit(1);
    }
    node->key=key;
//    node->tag[0]=node->tag[1]=THREAD;
    return node;
}

static void set_left_rotate(Node* node,Set* st)
{
    Node* right=node->child[1];
    node->child[1]=right->child[0];
    if((node->child[1]!=NULL))
        right->child[0]->parent=node;
    right->child[0]=node;
    right->parent=node->parent;
    if(right->parent!=NULL)
    {
        if(node==node->parent->child[1])
            node->parent->child[1]=right;
        else
            node->parent->child[0]=right;
    }
    else
    {
        st->root=right;
    }
    node->parent=right;
    return;
}

static void set_right_rotate(Node* node,Set* st)
{
    Node* left=node->child[0];
    node->child[0]=left->child[1];
    if((node->child[0]!=NULL))
        left->child[1]->parent=node;
    left->child[1]=node;
    left->parent=node->parent;
    if(left->parent!=NULL)
    {
        if(node==node->parent->child[0])
            node->parent->child[0]=left;
        else
            node->parent->child[1]=left;
    }
    else
    {
        st->root=left;
    }
    node->parent=left;
    return;
}

static void set_insert_maintain(Node* node,Set* st)
{
    Node *parent,*grandparent,*uncle,*tmp;
    parent=node->parent;
    while(parent!=NULL&&parent->color==RED)
    {
        grandparent=parent->parent;
        if(parent==grandparent->child[0])
        {
            uncle=grandparent->child[1];
            if(uncle!=NULL&&uncle->color==RED)
            {
                uncle->color=BLACK;
                parent->color=BLACK;
                grandparent->color=RED;
                node=grandparent;
            }
            else
            {
                if(parent->child[1]==node)
                {
                    set_left_rotate(parent,st);
                    tmp=parent;
                    parent=node;
                    node=tmp;
                }
                parent->color=BLACK;
                grandparent->color=RED;
                set_right_rotate(grandparent,st);
            }
        }
        else
        {
            uncle=grandparent->child[0];
            if(uncle!=NULL&&uncle->color==RED)
            {
                uncle->color=BLACK;
                parent->color=BLACK;
                grandparent->color=RED;
                node=grandparent;
            }
            else
            {
                if(parent->child[0]==node)
                {
                    set_right_rotate(parent,st);
                    tmp=parent;
                    parent=node;
                    node=tmp;
                }
                parent->color=BLACK;
                grandparent->color=RED;
                set_left_rotate(grandparent,st);
            }
        }
        parent=node->parent;
    }
    st->root->color=BLACK;
    return;
}

static void set_erase_maintain(Node *node,Node *parent,Set* st)
{
    Node *cousin,*left,*right;
    while((node!=NULL||node->color==BLACK)&&node!=st->root)
    {
        if(parent->child[0]==node)
        {
            cousin=parent->child[1];
            if(cousin->color==RED)
            {
                cousin->color=BLACK;
                parent->color=RED;
                set_left_rotate(parent,st);
                cousin=parent->child[1];
            }
            if((cousin->child[0]!=NULL||cousin->child[0]->color==BLACK)&&(cousin->child[1]!=NULL||cousin->child[1]->color==BLACK))
            {
                cousin->color=RED;
                node=parent;
                parent=node->parent;
            }
            else
            {
                if(cousin->child[1]!=NULL||cousin->child[1]->color==BLACK)
                {
                    left=cousin->child[0];
                    if(left!=NULL)
                    {
                        left->color=BLACK;

                    }
                    cousin->color=RED;
                    set_right_rotate(cousin,st);
                    cousin=parent->child[1];
                }
                cousin->color=parent->color;
                parent->color=BLACK;
                if(cousin->child[1]!=NULL)
                {
                    cousin->child[1]->color=BLACK;
                }
                set_left_rotate(parent,st);
                break;
            }
        }
        else
        {
            cousin=parent->child[0];
            if(cousin->color==RED)
            {
                cousin->color=BLACK;
                parent->color=RED;
                set_right_rotate(parent,st);
                cousin=parent->child[0];
            }
            if((cousin->child[0]!=NULL||cousin->child[0]->color==BLACK)&&(cousin->child[1]!=NULL||cousin->child[1]->color==BLACK))
            {
                cousin->color=RED;
                node=parent;
                parent=node->parent;
            }
            else
            {
                if(cousin->child[0]!=NULL||cousin->child[0]->color==BLACK)
                {
                    right=cousin->child[1];
                    if(left!=NULL)
                    {
                        left->color=BLACK;

                    }
                    cousin->color=RED;
                    set_left_rotate(cousin,st);
                    cousin=parent->child[0];
                }
                cousin->color=parent->color;
                parent->color=BLACK;
                if(cousin->child[0]!=NULL)
                {
                    cousin->child[0]->color=BLACK;
                }
                set_right_rotate(parent,st);
                break;
            }

        }
    }
    if(node)
    {
        node->color=BLACK;
    }
    return;
}

static Node* set_search(SET_TYPE key,Set *st,Node **closed)
{
    Node *node=st->root;
    Node *parent=NULL;
    while(node)
    {
        parent=node;
        if(node->key<key)
        {
           // if(node->tag[1])
           //     break;
            node=node->child[1];
        }
        else
            if(node->key>key)
            {
           //     if(node->tag[0])
           //         break;
                node=node->child[0];
            }
            else
                return node;
    }
    if(closed!=NULL)
        *closed=parent;
    return node;
}

void set_insert(Set* st,SET_TYPE value)
{
    Node *parent,*node;
    parent=NULL;
    node=set_search(value,st,&parent);
    if(node!=NULL)
        return;
    node=set_create_new_node(value);
    node->parent=parent;
    node->child[0]=node->child[1]=NULL;
    node->color=RED;
    if(parent!=NULL)
    {
        if(parent->key<value)
        {
    //        parent->tag[1]=REAL;
    //        node->child[0]=parent;
    //        node->child[1]=parent->child[1];
            parent->child[1]=node;
        }
        else
        {
    //        parent->child[0]=REAL;
    //        node->child[1]=parent;
    //        node->child[0]=parent->child[0];
            parent->child[0]=node;
        }
    }
    else
    {
        st->root=node;
        st->first=node;
        st->last=node;
    //    node->parent=st->root;
    //    node->tag[0]=node->tag[1]=THREAD;
    //    st->first=node;
    //    st->last=node;
    //    node->child[0]=st->first;
    //    node->child[1]=st->last;
    }
    ++st->size;
    set_insert_maintain(node,st);
    if(node->key<st->first->key)
        st->first=node;
    else
        if(node->key>st->last->key)
            st->last=node;
    return;
}

void set_erase(Set* st,SET_TYPE value)
{
    Node *child,*parent,*node,*left=NULL,*right=NULL,*record;
    bool color;
    node=set_search(value,st,NULL);
    if(node==NULL)
        return;
    if(node==st->first||node==st->last)
    {
        SetIterator iter;
        if(node==st->first)
        {
            iter.pointer=node,iter.set=st;
            SetIterator_self_increase(&iter);
            st->first=iter.pointer;
        }
        else
        {
            iter.pointer=node,iter.set=st;
            SetIterator_self_decrease(&iter);
            st->last=iter.pointer;
        }

    }
    record=node;
    if(node->child[0]!=NULL&&node->child[1]!=NULL)
    {
        node=node->child[1];
        left=node->child[0];
        while(left!=NULL)
        {
            node=left;
            left=node->child[0];
        }
        child=node->child[1];
        parent=node->parent;
        color=node->color;
        if(child!=NULL)
            child->parent=parent;
        if(parent!=NULL)
        {
            if(parent->child[0]==node)
                parent->child[0]=child;
            else
                parent->child[1]=child;
        }
        else
            st->root=child;
        if(node->parent==record)
            parent=node;
        node->parent=record->parent;
        node->color=record->color;
        node->child[1]=record->child[1];
        node->child[0]=record->child[0];
        if(record->parent!=NULL)
        {
            if(record->parent->child[0]==record)
                record->parent->child[0]=node;
            else
                record->parent->child[1]=node;
        }
        else
            st->root=node;
        record->child[0]->parent=node;
        if(record->child[1])
            record->child[1]->parent=node;
    }
    else
    {
        if(node->child[0]==NULL)
            child=node->child[1];
        else
            if(node->child[1]==NULL)
                child=node->child[0];
        parent=node->parent;
        color=node->color;
        if(child!=NULL)
            child->parent=parent;
        if(parent!=NULL)
        {
            if(parent->child[0]==node)
                parent->child[0]=child;
            else
                parent->child[1]=child;
        }
        else
            st->root=child;
    }
    free(record);
    if(color==BLACK)
        set_erase_maintain(child,parent,st);
    --st->size;
    return;
}
/*
void set_erase(Set* st,SET_TYPE value)
{
    Node *node,*parent;
    node=set_search(value,st,&parent);
    if(node==NULL)
        return;
    bool dir=parent->child[1]==node;
    parent=node->parent;
    if(node->tag[1]==THREAD)
    {
        if(node->tag[0]==REAL)
        {
            Node* child=node->child[0];
            while(child->tag[1]==REAL)
                child=child->child[1];
            child->child[1]=node->child[1];
            parent->child[dir]=node->child[0];
        }
        else
        {
            parent->child[dir]=node->child[dir];
            if(parent!=st->root)
                parent->tag[dir]=THREAD;
        }
    }
    else
    {
        Node* child=node->child[1];
        if(child->tag[0]==THREAD)
        {
            child->child[0]=node->child[0];
            child->tag[0]=node->tag[0];
            if(node->tag[0]==REAL)
            {
                Node* grandchild=child->child[0];
                while(grandchild->tag[1]==REAL)
                    grandchild=grandchild->child[1];
                grandchild->child[1]=child;
            }
            parent->child[dir]=child;
        }
        else
        {
            Node* next_node;
            for(;;)
            {
                next_node=child->child[0];
                if(next_node->tag[0]==THREAD)
                    break;
                child=next_node;
            }
            if(next_node->tag[1]==REAL)
                child->child[0]=next_node->child[1];
            else
            {
                child->child[0]=next_node;
                child->tag[0]=THREAD;
            }
            next_node->child[0]=node->child[0];
            if(node->tag[0]==REAL)
            {
                Node* temp=node->child[0];
                while(temp->tag[1]==REAL)
                    temp=temp->child[1];
                temp->child[1]=next_node;
                next_node->tag[0]=REAL;
            }
            next_node->child[1]=node->child[1];
            next_node->tag[1]=REAL;
            parent->child[dir]=next_node;
        }
    }
    free(node);
    set_erase_maintain(parent->child[dir],parent,st);
    --st->size;
    return;
}*/

bool set_empty(Set* st)
{
    return st->size==0;
}

int set_size(Set *st)
{
    return st->size;
}

void set_init(Set* st)
{
    st->size=0;
    st->root=NULL;
    return;
}

static void set_delete(Node* node)
{
    if(node==NULL)
        return;
    if(node->child[0]!=NULL)
        set_delete(node->child[0]);
    if(node->child[1]!=NULL)
        set_delete(node->child[1]);
    free(node);
    return;
}

void set_clear(Set* st)
{
    if(st->root!=NULL)
    {
        set_delete(st->root);
    }
    st->size=0;
    return;
}

SetIterator set_begin(Set* st)
{
    return (SetIterator){st->first,st};
}

SetIterator set_end(Set* st)
{
    return (SetIterator){st->last,st};
}

static void set_begin_init(Set* st)
{
    Node* node=st->root;
    if(node!=NULL)
        while(node->child[0])
            node=node->child[0];
    st->first=node;
}

static void set_end_init(Set* st)
{
    Node* node=st->root;
    if(node!=NULL)
        while(node->child[1])
            node=node->child[1];
    st->last=node;
}

SetIterator set_find(Set* st,SET_TYPE value)
{
    SetIterator iter;
    iter.pointer=set_search(value,st,NULL);
    iter.set=st;
    return iter;
}

SetIterator set_lower_bound(Set* st,SET_TYPE value)
{
    if(set_empty(st))
        return (SetIterator){NULL,st};
    Node *parent;
    Node *node=set_search(value,st,&parent);
    if(node==NULL)
        node=parent;
    SetIterator iter=(SetIterator){node,st};
    if(node->key>=value)
        return iter;
    else
    {
        SetIterator_self_increase(&iter);
        return iter;
    }
}

SetIterator set_upper_bound(Set* st,SET_TYPE value)
{
    if(set_empty(st))
        return (SetIterator){NULL,st};
    Node *parent;
    Node *node=set_search(value,st,&parent);
    if(node==NULL)
        node=parent;
    SetIterator iter=(SetIterator){node,st};
    if(node->key>value)
        return iter;
    else
    {
        SetIterator_self_increase(&iter);
        return iter;
    }
}

void SetIterator_self_increase(SetIterator* pIter)
{
    Node* node=pIter->pointer;
    Node* parent=node->parent;
    if(node==set_end(pIter->set).pointer)
        return;
    if(node->child[1]!=NULL)
    {
        node=node->child[1];
        while(node->child[0]!=NULL)
            node=node->child[0];
    }
    else
    {
        parent=node->parent;
        while(parent!=NULL&&node==parent->child[1])
        {
            node=parent;
            parent=node->parent;
        }
        node=parent;
    }
    pIter->pointer=node;
    return;
}

void SetIterator_self_decrease(SetIterator* pIter)
{
    Node* node=pIter->pointer;
    Node* parent=node->parent;
    if(node==set_begin(pIter->set).pointer)
        return;
    if(node->child[0]!=NULL)
    {
        node=node->child[0];
        while(node->child[1]!=NULL)
            node=node->child[1];
    }
    else
    {
        parent=node->parent;
        while(parent!=NULL&&node==parent->child[0])
        {
            node=parent;
            parent=node->parent;
        }
        node=parent;
    }
    pIter->pointer=node;
    return;
}

bool SetIterator_equal(SetIterator iter_a,SetIterator iter_b)
{
    return (iter_a.pointer==iter_b.pointer)&&(iter_a.set==iter_b.set);
}

bool SetIterator_not_equal(SetIterator iter_a,SetIterator iter_b)
{
    return !SetIterator_equal(iter_a,iter_b);
}

const SET_TYPE* set_get_data_ptr(SetIterator* iter)
{
    return &(iter->pointer->key);
}
