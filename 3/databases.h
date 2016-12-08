#pragma once
#include "types.h"

redisReply *CommandGet(redisDatabase *d,string s);
redisReply *CommandDel(redisDatabase *d,vector<string> arg);
redisReply *CommandExists(redisDatabase *d,vector<string> arg);
redisReply *CommandSet(redisDatabase *d,string s1,string s2);
