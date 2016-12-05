#ifndef _TYPES_H
#include "types.h"
#endif
redisReply CommandGet(redisDatabase *d,string s);
redisReply CommandDel(redisDatabase *d,string s);
redisReply CommandExists(redisDatabase *d,string s);
redisReply CommandSet(redisDatabase *d,string s1,string s2);
