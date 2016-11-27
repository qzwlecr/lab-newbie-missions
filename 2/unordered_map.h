
#include <stdbool.h>
#include <stdlib.h>
typedef struct Unordered_map Unordered_map;
typedef long long UNORDERED_MAP_KEY;
typedef double UNORDERED_MAP_TYPE;


// functions
// note the absence of reference
bool unordered_map_empty(Unordered_map* st);
int unordered_map_size(Unordered_map* st);

void unordered_map_init(Unordered_map* st);
void unordered_map_clear(Unordered_map* st);

void unordered_map_insert(Unordered_map* st, UNORDERED_MAP_KEY key, UNORDERED_MAP_TYPE value);
void unordered_map_erase(Unordered_map* st, UNORDERED_MAP_KEY key);

// return NULL if not found
// otherwise return pointer to value
UNORDERED_MAP_TYPE* unordered_map_ptr_at(Unordered_map* st, UNORDERED_MAP_KEY key);
