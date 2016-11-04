#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<magic.h>
#include<sys/types.h>
#include<dirent.h>
#include<hiredis/hiredis.h>

#define buf_size 1<<15

redisContext *Connect_text;
redisReply *reply;
magic_t cookie;
FILE * fp;
int cnt=0;  
void stradd(char* str, char c)
{
    int n=strlen(str);
    str[n]=c;
    str[n+1]='\0';
}

void Put_into_database(char *Location)
{
    fp=fopen(Location,"r");
    char word[buf_size];
    char buf=fgetc(fp);
    while(buf!=EOF)
    {
        if((buf>='a'&&buf<='z')||(buf>='0'&&buf<='9')||buf=='_')
            stradd(word,buf);
        else
        {
            if(strlen(word)>0)
            {
                reply=redisCommand(Connect_text,"ZINCRBY %s 1 ""%s""",word,Location);
                //printf("%s:%s\n",word,reply->str);
                memset(word,0,sizeof(word));
            }
        }
        buf=fgetc(fp);
    }
    if(strlen(word)>0)
    {
        reply=redisCommand(Connect_text,"ZINCRBY %s 1 ""%s""",word,Location);
        //printf("%s:%s\n",word,reply->str);
        memset(word,0,sizeof(word));
    }
    fclose(fp);
    return;
}

int List_Files(char *Location)
{
    int Location_length=strlen(Location);
    DIR * dir = opendir(Location);
    struct dirent *son;
    while((son = readdir(dir)) != NULL)
    {
        if(strncmp(son->d_name,".",1)==0)
            continue;
        if(son->d_type == DT_DIR)
        {
            int Son_length = strlen(son->d_name);
            strncpy(Location+Location_length,son->d_name,Son_length);
            Location[Location_length+Son_length]='/';
            Location[Location_length+Son_length+1]='\0';
            List_Files(Location);
            for(int i=Location_length;i<=Location_length+Son_length;++i)
                Location[i]='\0';
        }
        else
            if(son->d_type == DT_REG)
            {
                int Son_length = strlen(son->d_name);
                cookie=magic_open(MAGIC_MIME_TYPE);
                magic_load(cookie,NULL);
                strncpy(Location+Location_length,son->d_name,Son_length);
                Location[Location_length+Son_length]='\0';
                if(strncmp(magic_file(cookie,Location),"text/",5)==0)
                {
                    Put_into_database(Location);
                    printf("%d\n",++cnt);
                }
                for(int i=Location_length;i<=Location_length+Son_length;++i)
                    Location[i]='\0';
                magic_close(cookie);
            }
            else
                continue;
    }
    closedir(dir);
    return 0;
}

int main(int argc,char **argv)
{
    if(argc!=2)
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
    redisCommand(Connect_text,"FLUSHALL");
    int argv_length=strlen(argv[1]);
    if(argv[1][argv_length-1]!='/')
    {
        argv[1][argv_length]='/';
        argv[1][argv_length+1]='\0';
    }
    int List_error = List_Files(argv[1]);
    if(List_error)
        printf("Error code = %d\n!",List_error);
    else
        printf("Search completed!\n");
    redisFree(Connect_text);
    return 0;
}


/*连接数据库
redisContext *redisConnect(const char *ip, int port);
发送命令请求
void *redisCommand(redisContext *c, const char *format, ...);
void *redisCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);
void redisAppendCommand(redisContext *c, const char *format, ...);
void redisAppendCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);
释放资源
void freeReplyObject(void *reply);
void redisFree(redisContext *c);*/