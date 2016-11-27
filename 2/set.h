
#include <stdbool.h>
#include <stdlib.h>
typedef struct Set Set;
typedef struct SetIterator SetIterator;

typedef int SET_TYPE;


// functions
// note the absence of reference
bool set_empty(Set* st);
int set_size(Set* st);

void set_init(Set* st);
void set_clear(Set* st);

void set_insert(Set* st, SET_TYPE value);
void set_erase(Set* st, SET_TYPE value);


SetIterator set_begin(Set* st);
SetIterator set_end(Set* st);
SetIterator set_find(Set* st, SET_TYPE value);
SetIterator set_lower_bound(Set* st, SET_TYPE value);
SetIterator set_upper_bound(Set* st, SET_TYPE value);


void SetIterator_self_increase(SetIterator* pIter);
void SetIterator_self_decrease(SetIterator* pIter);

// iterators is of the same location.
bool SetIterator_equal(SetIterator iter_a, SetIterator iter_b);
bool SetIterator_not_equal(SetIterator iter_a, SetIterator iter_b);

const SET_TYPE* set_get_data_ptr(SetIterator* iter);
