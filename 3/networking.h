#pragma once
#include "type.h"

redisServer* redisServerInit();
redisClient *redisClientInit(int fd,redisServer *s);
void NetworkingRead(list<redisClient>::iterator iter);
void NetworkingListenAndAccept(redisServer *s);
redisReply *redisCommand(redisData *dt,redisClient *c);
redisReply *CommandMulti(redisClient *c);
redisReply *CommandExec(redisClient *c);
