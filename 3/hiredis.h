#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define REDIS_REPLY_INTEGER 1
#define REDIS_REPLY_STRING 2
#define REDIS_REPLY_NIL 3

typedef struct redisContext
{
    int err;
    char errstr[128];
    int fd;
    int flags;
    char *obuf;
}redisContext;

typedef struct redisReply
{
    int type;
    int integer;
    char *str;
}redisReply;

redisContext *redisConnect(const char *ip,int port);
void *redisCommand(redisContext *c,const char *format,...);
void freeReplyObject(void *reply);
void redisFree(redisContext *c);

