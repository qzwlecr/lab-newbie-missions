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
    int listenfd;
    if((listenfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        cout<<"init error!"<<endl;
        exit(1);
    }
    //cout<<listenfd<<' '<<errno<<endl;
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(2345);
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    int on=1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
    {
        cout<<"set socket opt error!"<<endl;
        cout<<errno<<endl;
        exit(2);
    }
    if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
    {
        cout<<"bind error"<<endl;
        exit(3);
    }
    if(listen(listenfd,SOMAXCONN)<0)
    {
        cout<<"listen error"<<endl;
        exit(4);
    }
    struct sockaddr_in peeraddr;
    socklen_t peerlen=sizeof(peeraddr);
    int conn;
    int client[10];
    int maxn;
    for(int i=0;i<10;++i)
        client[i]=-1;
    int nready;
    int maxfd=listenfd;
    fd_set rset;
    fd_set allset;
    FD_ZERO(&rset);
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);
    while(1)
    {
        rset=allset;
        nready=select(maxfd+1,&rset,NULL,NULL,NULL);
        if(nready<=0)
        {
            continue;
        }
        if(FD_ISSET(listenfd,&rset))
        {
            conn=accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen);
            if(conn==-1)
            {
                cout<<"accept error"<<endl;
                exit(5);
            }
            for(int i=0;i<10;++i)
            {
                if(client[i]<0)
                {
                    client[i]=conn;
                    if(i>maxn)
                        maxn=i;
                    break;
                }
            }
            printf("ip = %s , port = %d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
            FD_SET(conn,&allset);
            if(conn>maxfd)
                maxfd=conn;
            if(--nready<=0)
                continue;
        }
        for(int i=0;i<=maxn;++i)
        {
            conn=client[i];
            if(conn<0)
                continue;
            if(FD_ISSET(conn,&rset))
            {
                char recvbuf[10000];
                int ret=read(conn,recvbuf,1024);
                if(ret==-1)
                {
                    cout<<"read error"<<endl;
                    exit(6);
                }
                else
                    if(ret==0)
                    {
                        printf("client close\n");
                        FD_CLR(conn,&allset);
                        client[i]=-1;
                        close(conn);
                    }
                fputs(recvbuf,stdout);
            }
        }
    }
    return 0;
}
