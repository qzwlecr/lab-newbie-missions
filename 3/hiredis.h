#ifndef _TYPES_H
#include "types.h"
#endif
redisContext *redisConnect(const char *ip,int port);
redisReply *redisCommand(redisContext *c,const char *format,...);
void freeReplyObject(void *reply);
void redisFree(redisContext *c);

