#include "networking.h"
#include "databases.h"
#include "types.h"

int main()
{
    redisServer *s;
    s=NetworkingInit();
    if(s==NULL)
    {
        printf("networking init error:%s\n",strerror(errno));
        return 0;
    }
    while(1)
        NetworkingListenAndAccept(s);
}
