#include <stdbool.h>
#include <stdlib.h>
typedef double QUEUE_TYPE;
// TIPS: re-define the above identifiers
// in *.cpp file to make them shorter
// e.g. typedef T = QUEUE_TYPE;
// NOTE YOU CAN ONLY DO THAT IN *.cpp FILES

typedef struct Queue Queue;

// functions
// note the absence of reference
void queue_init(Queue* que);
bool queue_empty(Queue* que);
int queue_size(Queue* que);

void queue_clear(Queue* que);

void queue_push(Queue* que, QUEUE_TYPE value);
void queue_pop(Queue* que);
QUEUE_TYPE queue_top(Queue* que);
