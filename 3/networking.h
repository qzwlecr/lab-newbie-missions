#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <malloc.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

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

redisServer* NetworkingInit();
void NetworkingRead(int fd);
void NetworkingListenAndAccept(redisServer *s);
