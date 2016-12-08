#pragma once
#include "types.h"

redisServer* NetworkingInit();
void NetworkingRead(int fd,int loc,redisServer *s);
void NetworkingListenAndAccept(redisServer *s);
