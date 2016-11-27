
#include <stdbool.h>
#include <stdlib.h>
typedef struct List List;
typedef struct ListIterator ListIterator;

typedef double LIST_TYPE;


// functions
// note the absence of reference
LIST_TYPE* list_ptr_at(List* lis);
bool list_empty(List* lis);
int list_size(List* lis);

void list_init(List* lis);
void list_clear(List* lis);

//      a, b, c, ...,m, where, n, ...
// =>   a, b, c, ...,m, value, where, n, ...
void list_insert(List* lis, ListIterator where, LIST_TYPE value);
void list_erase(List* lis, ListIterator where);

void list_push_back(List* lis, LIST_TYPE value);
void list_pop_back(List* lis);
void list_push_front(List* lis, LIST_TYPE value);
void list_pop_front(List* lis);


ListIterator list_begin(List* lis);
ListIterator list_end(List* lis);
void ListIterator_self_increase(ListIterator* pIter);
void ListIterator_self_decrease(ListIterator* pIter);

ListIterator list_begin(List* lis);
ListIterator list_end(List* lis);
bool ListIterator_equal(ListIterator iter_a, ListIterator iter_b);
bool ListIterator_not_equal(ListIterator iter_a, ListIterator iter_b);

// you have to write like:
// ---------------------------------------
// LIST_TYPE value;
// *list_get_data_ptr(some_iter) = value;
// ---------------------------------------
LIST_TYPE* list_get_data_ptr(ListIterator* iter);
