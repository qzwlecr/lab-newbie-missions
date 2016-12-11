#pragma once
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <list>
#include <vector>
#include <malloc.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unordered_map>

using namespace std;

typedef struct redisDatabase redisDatabase;
typedef struct redisServer redisServer;
typedef struct redisContext redisContext;
typedef struct redisClient redisClient;
typedef struct redisReply redisReply;
typedef struct redisData redisData;

//redisDatabase
typedef struct redisDatabase
{
    unordered_map<string,string>* data;
}redisDatabase;

//redisServer
const int CLIENTS_MAX_NUMBER=1024;
const int SERVER_DEFAULT_PORT=6379;

typedef struct redisServer
{
    int listen_fd;
    int max_fd;
    fd_set all_set;
    list<redisClient>* clients;
    redisDatabase data;
}redisServer;

//redisContext and redisReply
const int REDIS_REPLY_NIL=0;
const int REDIS_REPLY_INTEGER=1;
const int REDIS_REPLY_STRING=2;
const int REDIS_REPLY_ERROR=3;
const int REDIS_REPLY_ARRAY=4;
const int REDIS_ERR=1;
const int REDIS_OK=2;
const int OBUF_INIT_SIZE=1024;
const int REPLY_INIT_SIZE=1024;

typedef struct redisContext
{
    int err;
    char errstr[128];
    int fd;
    string obuf;
}redisContext;

typedef struct redisReply
{
    int type;
    int integer;
    int elements;
    int len;
    char *str;
    struct redisReply **element;
}redisReply;

//redisClient
typedef struct redisClient
{
    int fd;
    redisServer *server;
    vector<redisData>* commands;
    sockaddr_in client;
    bool multiflag;
}redisClient;

//redisDataFirst

const int REDIS_COMMAND_SET=1;
const int REDIS_COMMAND_EXISTS=2;
const int REDIS_COMMAND_DEL=3;
const int REDIS_COMMAND_GET=4;
const int REDIS_COMMAND_MULTI=5;
const int REDIS_COMMAND_EXEC=6;

typedef struct redisData
{
    int type;
    vector<string>* args;
}redisData;
