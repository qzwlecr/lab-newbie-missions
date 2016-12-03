#include <cstdlib>
#include <cstring>
#include <string>
#include <malloc.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unordered_map>

using namespace std;

//redisServer
#define CLIENTS_MAX_NUMBER 1024
#define SERVER_DEFAULT_PORT 6379

typedef struct redisServer
{
    int listen_fd;
    int max_fd;
    fd_set all_set;
    int ready;
    int max_ready;
    int clients[CLIENTS_MAX_NUMBER];
}redisServer;

//redisContext and redisReply

#define REDIS_REPLY_INTEGER 1
#define REDIS_REPLY_STRING 2
#define REDIS_REPLY_NIL 3
#define REDIS_REPLY_ERROR 4

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

//redisDatabase
typedef struct redisDatabase
{
    unordered_map<string,string> data;
}redisDatabase;


