#include "networking.h"
#include "databases.h"

redisServer* redisServerInit()
{
    redisServer *s;
    s=(redisServer*)malloc(sizeof(redisServer));
    if((s->listen_fd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        free(s);
        return NULL;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(SERVER_DEFAULT_PORT);
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(s->listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        free(s);
        return NULL;
    }
    if(listen(s->listen_fd,SOMAXCONN)<0)
    {
        free(s);
        return NULL;
    }
    FD_ZERO(&s->all_set);
    FD_SET(s->listen_fd,&s->all_set);
    s->max_fd=s->listen_fd;
    s->data.data=new unordered_map<string,string>();
    s->clients=new list<redisClient>();
    return s;
}

redisClient *redisClientInit(int fd,redisServer *s)
{
    redisClient *c;
    c=(redisClient*)malloc(sizeof(redisClient));
    c->multiflag=false;
    c->fd=fd;
    c->server=s;
    c->commands=new vector<redisData>();
    return c;
}

void NetworkingListenAndAccept(redisServer *s)
{
    static sockaddr_in peer_addr;
    static socklen_t length=sizeof(peer_addr);
    int connected;
    fd_set temp=s->all_set;
    //printf("before\n");
    int ready=select(s->max_fd+1,&temp,NULL,NULL,NULL);
    //printf("after\n");
    //printf("ready number:%d\n",s->clients->size());
    if(ready<=0)
        return;
    if(FD_ISSET(s->listen_fd,&temp))
    {
        //cout<<"start to accept"<<endl;
        connected=accept(s->listen_fd,(struct sockaddr*)&peer_addr,&length);
        if(connected==-1)
            return;
        //cout<<"connected"<<endl;
        redisClient *c=redisClientInit(connected,s);
        s->clients->push_back(*c);
        FD_SET(connected,&s->all_set);
        if(connected>s->max_fd)
            s->max_fd=connected;
        if(--ready<=0)
            return;
    }
    for(list<redisClient>::iterator iter=s->clients->begin();iter!=s->clients->end();)
    {
        if(FD_ISSET((*iter).fd,&temp))
        {
            list<redisClient>::iterator next=iter;
            ++next;
            NetworkingRead(iter);
            iter=next;
        }
    }
    return;
}

void NetworkingRead(list<redisClient>::iterator iter)
{
    redisReply *r;
    redisData *dt;
    char *buf;
    //cout<<"read init"<<endl;
    r=(redisReply*)malloc(sizeof(redisReply));
    int length,size;
    int ret=recv((*iter).fd,&length,sizeof(int),0);
    if(ret==-1)
    {
        return;
    }
    else if(ret==0)
    {
        //cout<<"no data"<<endl;
        close((*iter).fd);
        (*iter).server->clients->erase(iter);
        FD_CLR((*iter).fd,&(*iter).server->all_set);
        free(r);
        return;
    }
    buf=(char*)malloc(length+1);
    dt=(redisData*)malloc(sizeof(redisData));
    dt->args=new vector<string>();
    ret=recv((*iter).fd,buf,length,0);
    if(ret==-1)
    {
        return;
    }
    else if(ret==0)
    {
        //cout<<"no data"<<endl;
        close((*iter).fd);
        (*iter).server->clients->erase(iter);
        FD_CLR((*iter).fd,&(*iter).server->all_set);
        free(r);
        free(buf);
        delete(dt->args);
        free(dt);
        return;
    }
    //for(int i=0;i<length;++i)
    //    cout<<buf[i]<<endl;
    memcpy(&dt->type,buf,sizeof(int));
    memcpy(&size,buf+sizeof(int),sizeof(int));
    int ptr=sizeof(int)*2;
    //cout<<233<<endl;
    for(int i=0;i<size;++i)
    {
        int strlength;
        memcpy(&strlength,buf+ptr,sizeof(int));
        //cout<<strlength<<endl;
        ptr+=sizeof(int);
        string temp(buf+ptr,strlength);
        dt->args->push_back(temp);
        ptr+=strlength;
    }
    //cout<<"ready to command"<<endl;
    r=redisCommand(dt,&(*iter));
    //cout<<"command end"<<endl;
    free(buf);
    length=sizeof(redisReply)+r->len;
    for(int i=0;i<r->elements;++i)
        length+=sizeof(redisReply)+r->element[i]->len;
    buf=(char*)malloc(length);
    ptr=sizeof(redisReply);
    memcpy(buf,r,sizeof(redisReply));
    memcpy(buf+ptr,r->str,r->len);
    ptr+=r->len;
    for(int i=0;i<r->elements;++i)
    {
        memcpy(buf+ptr,r->element[i],sizeof(redisReply));
        ptr+=sizeof(redisReply);
        memcpy(buf+ptr,r->element[i]->str,r->element[i]->len);
        ptr+=r->element[i]->len;
    }
    //for(int i=0;i<length;++i)
    //{
    //    cout<<(int)buf[i]<<endl;
    //}
    //cout<<"???"<<endl;
    send((*iter).fd,&length,sizeof(int),0);
    send((*iter).fd,buf,length,0);
    delete dt->args;
    free(dt);
    return;
}

redisReply *redisCommand(redisData *dt,redisClient *c)
{
    redisReply *r;
    if(c->multiflag)
    {
        r=(redisReply*)malloc(sizeof(redisReply));
        c->commands->push_back(*dt);
        r->type=REDIS_REPLY_STRING;
        r->str=(char*)malloc(sizeof("QUEUED"));
        memcpy(r->str,"QUEUED",sizeof("QUEUED"));
        r->len=6;
    }
    else
    {
        switch(dt->type)
        {
            case REDIS_COMMAND_SET:
            {
                r=CommandSet(&c->server->data,(*dt->args)[0],(*dt->args)[1]);
                break;
            }
            case REDIS_COMMAND_EXISTS:
            {
                r=CommandExists(&c->server->data,(*dt->args));
                break;
            }
            case REDIS_COMMAND_GET:
            {
                r=CommandGet(&c->server->data,(*dt->args)[0]);
                break;
            }
            case REDIS_COMMAND_DEL:
            {
                r=CommandDel(&c->server->data,(*dt->args));
                break;
            }
            case REDIS_COMMAND_MULTI:
            {
                r=CommandMulti(c);
                break;
            }
            case REDIS_COMMAND_EXEC:
            {
                r=CommandExec(c);
                break;
            }

        }
    }
    return r;
}

redisReply *CommandMulti(redisClient *c)
{
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    r->type=REDIS_REPLY_STRING;
    r->str=(char*)malloc(sizeof("OK"));
    memcpy(r->str,"OK",sizeof("OK"));
    c->multiflag=true;
    return r;
}

redisReply *CommandExec(redisClient *c)
{
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    r->type=REDIS_REPLY_ARRAY;
    r->element=(redisReply**)malloc((c->commands->size()+1)*sizeof(redisReply*));
    c->multiflag=false;
    for(int i=0;i<c->commands->size();++i)
    {
        r->element[i]=redisCommand(&(*c->commands)[i],c);
    }
    r->elements=c->commands->size();
    return r;
}
