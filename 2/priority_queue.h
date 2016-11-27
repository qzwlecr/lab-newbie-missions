#include <stdbool.h>
#include <stdlib.h>
typedef double PRIORITY_QUEUE_TYPE;
// TIPS: re-define the above identifiers
// in *.cpp file to make them shorter
// e.g. typedef T = PRIORITY_QUEUE_TYPE;
// NOTE YOU CAN ONLY DO THAT IN *.cpp FILES

typedef struct Priority_queue Priority_queue;

// functions
// note the absence of reference
void priority_queue_init(Priority_queue* stk);
bool priority_queue_empty(Priority_queue* stk);
int priority_queue_size(Priority_queue* stk);

void priority_queue_clear(Priority_queue* stk);

void priority_queue_push(Priority_queue* stk, PRIORITY_QUEUE_TYPE value);
void priority_queue_pop(Priority_queue* stk);
// return pointer to make it mutable is meaningless.
PRIORITY_QUEUE_TYPE priority_queue_top(Priority_queue* stk);

