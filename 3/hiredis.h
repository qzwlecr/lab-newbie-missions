#include "types.h"

redisContext *redisConnect(const char *ip,int port);
void *redisCommand(redisContext *c,const char *format,...);
void freeReplyObject(void *reply);
void redisFree(redisContext *c);

