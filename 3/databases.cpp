#include "databases.h"

redisReply CommandGet(redisDatabase *d,string s)
{
    redisReply r;
    unordered_map<string,string>::iterator iter=d->data.find(s);
    if(iter==d->data.end())
    {
        r.type=REDIS_REPLY_NIL;
        return r;
    }
    r.type=REDIS_REPLY_STRING;
    r.str=(char*)malloc(sizeof(s.c_str()));
    strcpy(r.str,s.c_str());
    return r;
}

redisReply CommandExists(redisDatabase *d,string s)
{
    redisReply r;
    r.type=REDIS_REPLY_INTEGER;
    if(d->data.find(s)==d->data.end())
    {
        r.integer=0;
        return r;
    }
    else
    {
        r.integer=1;
        return r;
    }
}

redisReply CommandSet(redisDatabase *d,string s1,string s2)
{
    redisReply r;
    r.type=REDIS_REPLY_STRING;
    d->data[s1]=s2;
    r.str=(char*)malloc(sizeof("OK"));
    strcpy(r.str,"OK");
    return r;
}

redisReply CommandDel(redisDatabase *d,string s)
{
    redisReply r;
    r.type=REDIS_REPLY_INTEGER;
    r.integer=d->data.erase(s);
    return r;
}
