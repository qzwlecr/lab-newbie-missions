#pragma once
#include "types.h"

redisServer* NetworkingInit();
void NetworkingRead(int fd,redisServer *s);
void NetworkingListenAndAccept(redisServer *s);
