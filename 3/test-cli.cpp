#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
using namespace std;

int main()
{
    int sendfd;
    sendfd=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(2345);
    serveraddr.sin_addr.s_addr=inet_addr("192.168.2.213");
    connect(sendfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    char s[1024];
    while(1)
    {
        scanf("%s",s);
        int len=strlen(s);
        s[len]='\n';
        s[len+1]='\0';
        write(sendfd,s,sizeof(s));
    }
    return 0;
}
