#include"unordered_map.h"

#define HASH_INIT_SIZE 16

typedef struct Hash_Node
{
    UNORDERED_MAP_KEY key;
    UNORDERED_MAP_TYPE value;
    struct Hash_Node* next;
}Hash_Node;

typedef struct Unordered_map
{
    int size;
    int count;
    Hash_Node** hashnodes;
}Unordered_map;

static void unordered_map_resize(Unordered_map* st);

bool unordered_map_empty(Unordered_map* st)
{
    return (st->size==0);
}

int unordered_map_size(Unordered_map* st)
{
    return st->count;
}

void unordered_map_init(Unordered_map* st)
{
    st->size=HASH_INIT_SIZE;
    st->count=0;
    st->hashnodes=(Hash_Node**)calloc(st->size,sizeof(Hash_Node*));
    if(st->hashnodes==NULL)
    {
        free(st);
        exit(1);
    }
    return;
}

void unordered_map_clear(Unordered_map* st)
{
    Hash_Node *cur=NULL,*tmp=NULL;
    for(int i=0;i<st->size;++i)
    {
        cur=st->hashnodes[i];
        while(cur)
        {
            tmp=cur;
            cur=cur->next;
            free(tmp);
        }
    }
    free(st->hashnodes);
    return;
}

static int hashcode(long long key)
{
    key=(~key)+(key<<18);
    key=key^(key>>31);
    key=key*21;
    key=key^(key>>11);
    key=key+(key<<6);
    key=key^(key>>22);
    return (int)key;
}


void unordered_map_insert(Unordered_map* st,UNORDERED_MAP_KEY key,UNORDERED_MAP_TYPE value)
{
    if(st->size*0.75<st->count)
        unordered_map_resize(st);
    int index=hashcode(key);
    index=(index%st->size+st->size)%st->size;
    Hash_Node *old_hashnodes=st->hashnodes[index];
    Hash_Node *cur=old_hashnodes;
    while(cur)
    {
        if(key==cur->key)
        {
            cur->value=value;
            return;
        }
        cur=cur->next;
    }
    Hash_Node *new_node=(Hash_Node*)malloc(sizeof(Hash_Node));
    new_node->key=key;
    new_node->value=value;
    new_node->next=NULL;
    st->count+=1;
    if(old_hashnodes!=NULL)
        new_node->next=old_hashnodes;
    st->hashnodes[index]=new_node;
    return;
}

void unordered_map_erase(Unordered_map* st,UNORDERED_MAP_KEY key)
{
    int index=hashcode(key);
    index=(index%st->size+st->size)%st->size;
    Hash_Node *cur=st->hashnodes[index];
    Hash_Node *last=NULL;
    while(cur!=NULL)
    {
        if(cur==NULL)
            return;
        if(cur->key==key)
        {
            if(last==NULL)
                st->hashnodes[index]=cur->next;
            else
            {
                last->next=cur->next;
            }
            free(cur);
            return;
        }
        last=cur;
        cur=cur->next;
    }
    return;
}

static void unordered_map_resize(Unordered_map* st)
{
    int old_size=st->size;
    st->size*=2;
    st->count=0;
    Hash_Node **old_hashnodes=st->hashnodes;
    st->hashnodes=(Hash_Node**)calloc(st->size,sizeof(Hash_Node*));
    for(int i=0;i<old_size;++i)
    {
        Hash_Node *cur=old_hashnodes[i];
        Hash_Node *record;
        while(cur!=NULL)
        {
            unordered_map_insert(st,cur->key,cur->value);
            record=cur;
            cur=cur->next;
            free(record);
        }
    }
    free(old_hashnodes);
    return;
}

UNORDERED_MAP_TYPE* unordered_map_ptr_at(Unordered_map* st,UNORDERED_MAP_KEY key)
{
    int index=hashcode(key);
    index=(index%st->size+st->size)%st->size;
    Hash_Node *cur=st->hashnodes[index];
    if(cur==NULL)
        return NULL;
    while(cur!=NULL)
    {
        if(cur->key==key)
            return &cur->value;
        cur=cur->next;
    }
    return NULL;
}
