#include "sds.h"
#include "type.h"
static uint32_t countDigits(uint64_t v);
static size_t blklen(size_t len);
int redisvFormatCommand(char **target,const char *format,va_list ap);
int redisFormatCommand(char **target,const char *format,...);
