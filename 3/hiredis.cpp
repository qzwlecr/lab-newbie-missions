#include "hiredis.h"

redisContext *redisConnect(const char *ip,int port)
{
    redisContext *c;
    c=(redisContext*)malloc(sizeof(redisContext));
    if(c==NULL)
        return NULL;
    c->err=0;
    c->errstr[0]='\0';
    if((c->fd=socket(PF_INET,SOCK_STREAM,0))<0)
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
        strcpy(c->errstr,strerror(errno));
        return c;
    }
    return c;
}

redisReply *redisCommand(redisContext *c,const char *format,...)
{
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    if(r==NULL)
    {
        return NULL;
    }
    if(c->err)
    {
        r->type=REDIS_REPLY_ERROR;
        return r;
    }
    va_list arg_ptr;
    va_start(arg_ptr,format);
    vsnprintf(c->obuf,OBUF_INIT_SIZE,format,arg_ptr);
    va_end(arg_ptr);
    if(send(c->fd,c->obuf,strlen(c->obuf),0)==-1)
    {
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
        return r;
    }
    cout<<c->obuf<<endl;
    if(recv(c->fd,r,sizeof(redisReply),0)==-1)
    {
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
        return r;
    }
    return r;
}

void freeReplyObject(redisReply *reply)
{
    free(reply);
}

void redisFree(redisContext *c)
{
    if(c->fd>0)
        close(c->fd);
    free(c);
}
