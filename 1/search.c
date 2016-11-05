#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<magic.h>
#include<sys/types.h>
#include<dirent.h>
#include<hiredis/hiredis.h>

redisContext *Connect_text;
redisReply *reply;


int main(int argc,char **argv)
{
    if(argc==1)
    {
        printf("Not a correct format!\n");
        return 0;
    }
    Connect_text=redisConnect("127.0.0.1",6379);
    if(Connect_text==NULL||Connect_text->err)
    {
        printf("Can't connect to the database!\n");
        return 0;
    }
    redisCommand(Connect_text,"ZREMRANGEBYSCORE allcounter -inf +inf");
    redisCommand(Connect_text,"ZREMRANGEBYSCORE numbercounter -inf +inf");
    redisCommand(Connect_text,"ZREMRANGEBYSCORE answer -inf +inf");
    static char cursor_init[10];
    for(int i=1;i<argc;++i)
    {
        int cursor=0;
        int done=0;
        while(!done)
        {
            reply=redisCommand(Connect_text,"ZSCAN %s %d",argv[i],cursor);
            if(reply->type==REDIS_REPLY_ERROR)
            {
                printf("search error!\n");
                freeReplyObject(reply);
                redisFree(Connect_text);
                return 0;
            }
            if(reply->element[1]->elements==0)
            {
                printf("%s NOT FOUND!\n",argv[i]);
                break;
            }
            redisReply **site_Counters=reply->element[1]->element;
            strcpy(cursor_init,reply->element[0]->str);
            sscanf(cursor_init,"%d",&cursor);
            for(int j=0;j<reply->element[1]->elements;++j)
            {
                redisCommand(Connect_text,"ZINCRBY allcounter %s %s",site_Counters[j+1]->str,site_Counters[j]->str);
                redisCommand(Connect_text,"ZINCRBY numbercounter %d %s",1,site_Counters[j]->str);
                redisCommand(Connect_text,"HMSET words%d %s %s",i,site_Counters[j]->str,site_Counters[j+1]->str);
                ++j;
            }
            if(cursor==0)
                done=1;
        }
    }
    redisCommand(Connect_text,"ZUNIONSTORE answer 2 allcounter numbercounter WEIGHTS 0.1 1000000");
    reply=redisCommand(Connect_text,"ZREVRANGE answer 0 -1");
    for(int i=0;i<reply->elements;++i)
    {
        printf("%s ",reply->element[i]->str);
        redisReply *ans;
        ans=redisCommand(Connect_text,"ZSCORE numbercounter %s",reply->element[i]->str);
        printf("matches %s words in the list,",ans->str);
        ans=redisCommand(Connect_text,"ZSCORE allcounter %s",reply->element[i]->str);
        printf("and the number of all words is %s.\n",ans->str);
        for(int j=1;j<argc;++j)
        {
            ans=redisCommand(Connect_text,"HGET words%d %s",j,reply->element[i]->str);
            if(ans->type==REDIS_REPLY_NIL)
                printf("-----word %s appears 0 times.\n",argv[j]);
            else
                printf("-----word %s appears %s times.\n",argv[j],ans->str);
        }
        freeReplyObject(ans);
    }   
    for(int i=1;i<argc;++i)
        redisCommand(Connect_text,"DEL words%d",i);
    freeReplyObject(reply);
    redisFree(Connect_text);
    return 0;
}
