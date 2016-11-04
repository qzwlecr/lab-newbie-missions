#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<magic.h>
#include<sys/types.h>
#include<dirent.h>
#include<hiredis/hiredis.h>

redisContext *Connect_text;
redisReply *reply,*ans;

#define word_size 10

int main(int argc,char **argv)
{
    if(argc==1)
    {
        printf("Not a correct format!\n");
        return 0;
    }
    Connect_text = redisConnect("127.0.0.1",6379);
    if(Connect_text==NULL||Connect_text->err)
    {
        printf("Can't connect to the database!\n");
        return 0;
    }
    static char cursor_init[10];
    for(int i=1;i<argc;++i)
    {
        int cursor=0;
        int done=0;
        while(!done)
        {
            reply=redisCommand(Connect_text,"ZSCAN %s %d",argv[i],cursor);
            if(reply==NULL)
            {
                printf("search error!");
                redisFree(Connect_text);
                return 0;
            }
            redisReply **site_Counters=reply->element[1]->element;
            strcpy(cursor_init,reply->element[0]->str);
            sscanf(cursor_init,"%d",&cursor);
            for(int j=0;j<reply->element[1]->elements;++j)
            {
                redisCommand(Connect_text,"ZINCRBY allcounter %s %s",site_Counters[j+1]->str,site_Counters[j]->str);
                redisCommand(Connect_text,"ZINCRBY numbercounter %d %s",1,site_Counters[j]->str);
                redisCommand(Connect_text,"HINCRBY words%d %s %s",i,site_Counters[j]->str,site_Counters[j+1]->str);
                ++j;
            }
            if(cursor==0)
                done=1;
        }
    }
    redisCommand(Connect_text,"ZUNIONSTORE answer 2 allcounter numbercounter WEIGHTS 0.1 10000000");
    reply=redisCommand(Connect_text,"ZREVRANGE answer 0 -1");
    for(int i=0;i<reply->elements;++i)
    {
        printf("%s ",reply->element[i]->str);
        ans=redisCommand(Connect_text,"ZSCORE numbercounter %s",reply->element[i]->str);
        printf("matches %s words in the list,",ans->str);
        ans=redisCommand(Connect_text,"ZSCORE allcounter %s",reply->element[i]->str);
        printf("and the number of all words is %s.\n",ans->str);
        for(int j=1;j<argc;++j)
        {
            ans=redisCommand(Connect_text,"HGET words%d %s",j,reply->element[i]->str);
            if(ans==NULL)
                continue;
            else
                printf("-word %s appears %s times.",argv[j],ans->str);
        }
    }
    freeReplyObject(reply);
    freeReplyObject(ans);
    redisFree(Connect_text);
    return 0;
}