#include "networking.h"
#include "databases.h"
#include "type.h"

int main()
{
    redisServer *s;
    s=redisServerInit();
    if(s==NULL)
    {
        printf("networking init error:%s\n",strerror(errno));
        return 0;
    }
    while(1)
        NetworkingListenAndAccept(s);
}
