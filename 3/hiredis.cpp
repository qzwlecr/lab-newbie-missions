#include "hiredis.h"

redisContext *redisConnect(const char *ip,int port)
{
    redisContext *c;
    c=(redisContext*)malloc(sizeof(redisContext));
    if(c==NULL)
        return NULL;
    c->err=0;
    c->errstr[0]='\0';
    c->obuf=NULL;
    c->fd=socket(PF_INET,SOCK_STREAM,0);
    if((c->fd=socket(PF_INET,SOCK_STREAM,0)<0))
    {
        c->err=1;
        strcpy(c->errstr,"Can't get socket!");
        return c;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);
    if((connect(c->fd,(struct sockaddr*)&addr,sizeof(addr)))<0)
    {
        close(c->fd);
        c->fd=0;
        c->err=1;
        strcpy(c->errstr,"Can't establish connection!");
        return c;
    }
    return c;
}
static redisReply *redisFormatCommand()
{

}
redisReply *redisCommand()
{

}


void freeReplyObject(redisReply *reply)
{
    free(reply);
}

void redisFree(redisContext *c)
{
    if(c->fd>0)
        close(c->fd);
    if(c->obuf!=NULL)
        free(c->obuf);
    free(c);
}
