#include "types.h"

redisServer* NetworkingInit();
void NetworkingRead(int fd);
void NetworkingListenAndAccept(redisServer *s);
