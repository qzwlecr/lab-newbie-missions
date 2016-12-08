#include "databases.h"

redisReply *CommandGet(redisDatabase *d,string s)
{
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    unordered_map<string,string>::iterator iter=d->data->find(s);
    if(iter==d->data->end())
    {
        r->type=REDIS_REPLY_NIL;
        return r;
    }
    r->type=REDIS_REPLY_STRING;
    strcpy(r->str,(*iter).second.c_str());
    return r;
}

redisReply *CommandExists(redisDatabase *d,vector<string> arg)
{
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    r->type=REDIS_REPLY_INTEGER;
    r->integer=0;
    for(int cnt=0;cnt<arg.size();++cnt)
    {
        if(d->data->find(arg[cnt])!=d->data->end())
        {
            ++r->integer;
        }
    }
    return r;
}

redisReply *CommandSet(redisDatabase *d,string s1,string s2)
{
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    r->type=REDIS_REPLY_STRING;
    (*d->data)[s1]=s2;
    strcpy(r->str,"OK");
    return r;
}

redisReply *CommandDel(redisDatabase *d,vector<string> arg)
{
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    r->type=REDIS_REPLY_INTEGER;
    r->integer=0;
    for(int cnt=0;cnt<arg.size();++cnt)
        r->integer+=d->data->erase(arg[cnt]);
    return r;
}
