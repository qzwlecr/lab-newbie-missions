#include "hiredis.h"
#include <iostream>

using namespace std;

redisContext *c;
redisReply *r;
int main()
{
    c=redisConnect("127.0.0.1",SERVER_DEFAULT_PORT);
    if(c==NULL||c->err)
    {
        cout<<"error:"<<c->errstr<<endl;
        return 1;
    }
    r=redisCommand(c,"set a 233");
    if(c->err)
    {
        cout<<c->errstr<<endl;
        return 1;
    }
    if(r->type==REDIS_REPLY_ERROR)
    {
        cout<<strerror(errno)<<endl;
        return 1;
    }
    cout<<r->str<<endl;
    return 0;
}
