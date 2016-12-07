#include "networking.h"
#include "databases.h"
redisServer* NetworkingInit()
{
    redisServer *s;
    s=(redisServer*)malloc(sizeof(redisServer));
    if((s->listen_fd=socket(PF_INET,SOCK_STREAM,0))<0)\
    {
        free(s);
        return NULL;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(SERVER_DEFAULT_PORT);
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(s->listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        free(s);
        return NULL;
    }
    if(listen(s->listen_fd,SOMAXCONN)<0)
    {
        free(s);
        return NULL;
    }
    FD_ZERO(&s->all_set);
    FD_SET(s->listen_fd,&s->all_set);
    s->max_fd=s->listen_fd;
    s->max_ready=0;
    memset(s->clients,-1,sizeof(s->clients));
    s->data.data=new unordered_map<string,string>();
    return s;
}

void NetworkingListenAndAccept(redisServer *s)
{
    fd_set temp=s->all_set;
    static sockaddr_in peer_addr;
    int connected;
    socklen_t length=sizeof(peer_addr);
    //printf("before\n");
    s->ready=select(s->max_fd+1,&temp,NULL,NULL,NULL);
    //printf("after\n");
    //printf("ready number:%d\n",s->ready);
    if(s->ready<=0)
        return;
    if(FD_ISSET(s->listen_fd,&temp))
    {
    //    cout<<"start to accept"<<endl;
        connected=accept(s->listen_fd,(struct sockaddr*)&peer_addr,&length);
        if(connected==-1)
            return;
    //    cout<<"accepted"<<endl;
        for(int i=0;i<CLIENTS_MAX_NUMBER;++i)
        {
            if(s->clients[i]<0)
            {
                s->clients[i]=connected;
    //            cout<<i<<endl;
                if(i>s->max_ready)
                    s->max_ready=i;
                break;
            }
        }
        FD_SET(connected,&s->all_set);
        if(connected>s->max_fd)
            s->max_fd=connected;
        if(--s->ready<=0)
            return;
    }
    for(int i=0;i<=s->max_ready;++i)
    {
        connected=s->clients[i];
        if(connected<0)
            continue;
    //    cout<<connected<<" "<<endl;
        if(FD_ISSET(connected,&temp))
        {
            NetworkingRead(connected,s);
            close(connected);
            s->clients[i]=-1;
            FD_CLR(connected,&s->all_set);
        }
    }
    return;
}

void NetworkingRead(int fd,redisServer *s)
{
    static char ibuf[1024*16];
    int size;
    redisReply *r;
    r=(redisReply*)malloc(sizeof(redisReply));
    if((size=recv(fd,ibuf,sizeof(ibuf),0))==-1)
    {
        return;
    }
    stringstream ss;
    ss<<ibuf;
    string word;
    ss>>word;
    if(word[0]=='s'||word[0]=='S')
    {
    //    printf("233\n");
        string s1,s2;
        ss>>s1>>s2;
    //    cout<<s1<<' '<<s2<<endl;
        r=CommandSet(&s->data,s1,s2);
    }
    else if(word[0]=='d'||word[0]=='D')
    {
    //    printf("2333\n");
        string s1;
        ss>>s1;
    //    cout<<s1<<endl;
        r=CommandDel(&s->data,s1);
    }
    else if(word[0]=='e'||word[0]=='E')
    {
    //    printf("23333\n");
        string s1;
        ss>>s1;
    //    cout<<s1<<endl;
        r=CommandExists(&s->data,s1);
    }
    else if(word[0]=='g'||word[0]=='G')
    {
    //    printf("233333\n");
        string s1;
        ss>>s1;
    //    cout<<s1<<endl;
        r=CommandGet(&s->data,s1);
    }
    send(fd,r,sizeof(redisReply),0);
    return;
}
