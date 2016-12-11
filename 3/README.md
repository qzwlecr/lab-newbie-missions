#UniqueLab Mission 3

##介绍

本任务主要实现了一个类redis数据库。
redisContext \*redisConnect(const char \*ip,int port);
redisReply \*redisCommand(redisContext \*c,const char \*format,...);
void freeReplyObject(void \*reply);
void redisFree(redisContext \*c);

##编译方法

    makefile

##执行方法

    ./server

##调用接口方法

    #include "hiredis.h"
    编译时调用静态库libhiredis.a
    
##简介

本期的任务是实现Redis数据库的部分功能并提供C/C++接口。
不限制实现语言，但是平台仅限linux,特别是如果想选做分布式部分

基本要求：
数据类型仅限String一种,注意String是二进制安全的
要求实现以下操作：SET(无需实现过期时间),DEL,EXIST,GET
要求支持多客户端

可选要求：以下要求至少选做1个
1.通过实现MULTI/EXEC对原子操作提供支持
2.支持多个服务器组成分布式数据库
3.支持Hash,Set,List,SortedSet类型以及相关操作（不推荐只做这个）



