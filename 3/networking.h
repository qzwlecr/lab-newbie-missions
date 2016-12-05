#ifndef _TYPES_H
#include "types.h"
#endif
redisServer* NetworkingInit();
void NetworkingRead(int fd,redisServer *s);
void NetworkingListenAndAccept(redisServer *s);
