#include "hiredis.h"
#include <errno.h>

redisContext *redisConnect(const char *ip,int port)
{
    redisContext *c;
    c=(redisContext*)malloc(sizeof(redisContext));
    if(c==NULL)
        return NULL;
    c->err=0;
    c->errstr[0]='\0';
    c->obuf=NULL;
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
    if(c->err)
    {
        r->type=REDIS_REPLY_ERROR;
        return r;
    }
    int size=OBUF_INIT_SIZE;
    c->obuf=(char*)malloc(size*sizeof(char));
    if(c->obuf==NULL)
    {
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
        return r;
    }
    va_list arg_ptr;
    while(1)
    {
        va_start(arg_ptr,format);
        int already_print=vsnprintf(c->obuf,size,format,arg_ptr);
        if(already_print<=-1&&already_print>=size)
        {
            size<<=1;
            c->obuf=(char*)malloc(size*sizeof(char));
            if(c->obuf==NULL)
            {
                c->err=1;
                strcpy(c->errstr,strerror(errno));
                r->type=REDIS_REPLY_ERROR;
                return r;
            }
            continue;
        }
        else
            break;
    }
    va_end(arg_ptr);
    if(send(c->fd,c->obuf,strlen(c->obuf),0)==-1)
    {
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
        return r;
    }
    free(c->obuf);
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
    if(reply->str!=NULL)
        free(reply->str);
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
