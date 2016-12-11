#include "hiredis.h"

redisContext *redisConnect(const char *ip,int port)
{
    redisContext *c;
    c=(redisContext*)malloc(sizeof(redisContext));
    if(c==NULL)
        return NULL;
    c->err=0;
    c->errstr[0]='\0';
    if((c->fd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        c->err=1;
        strcpy(c->errstr,"Can't get socket!");
        return c;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);
    if((connect(c->fd,(struct sockaddr*)&addr,sizeof(addr)))<0)
    {
        close(c->fd);
        c->fd=0;
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        return c;
    }
    return c;
}

redisReply *redisCommand(redisContext *c,const char *format,...)
{
    redisReply *r;
    redisData *dt;
    r=(redisReply*)malloc(sizeof(redisReply));
    dt=(redisData*)malloc(sizeof(redisData));
	dt->args=new vector<string>();
    char *buf;
    string origin;
	if(r==NULL)
    {
        return NULL;
    }
    if(c->err)
    {
        r->type=REDIS_REPLY_ERROR;
        return r;
    }
    va_list arg_ptr;
    va_start(arg_ptr,format);
    //vsnprintf(c->obuf,OBUF_INIT_SIZE,format,arg_ptr);
    //cout<<c->obuf<<endl;
	int length=redisvFormatCommand(&buf,format,arg_ptr);
	va_end(arg_ptr);
    for(int i=0;i<length;++i)
    {
        origin+=buf[i];
    }
    string get=origin;
    int argc;
    sscanf(buf,"*%d",&argc);
	//cout<<argc<<endl;
    get=get.substr(get.find_first_of("\r\n")+2);
	//cout<<get<<endl;
    get=get.substr(get.find_first_of("\r\n")+2);
	//cout<<get<<endl;
    get=get.substr(get.find_first_of("\r\n")+2);
	//cout<<get<<endl;
    if(format[0]=='E'&&format[2]=='I')
    {
        dt->type=REDIS_COMMAND_EXISTS;
        for(int i=0;i<argc-1;++i)
        {
			length=atoi(get.substr(1,get.find_first_of("\r\n")-1).data());
            get=get.substr(get.find_first_of("\r\n")+2);
            string key=get.substr(0,length);
            dt->args->push_back(key);
			get=get.substr(length+2);
        }
    }
	else if(format[0]=='E'&&format[2]=='E')
	{
		dt->type=REDIS_COMMAND_EXEC;
	}
	else if(format[0]=='G')
	{
		dt->type=REDIS_COMMAND_GET;
		length=atoi(get.substr(1,get.find_first_of("\r\n")-1).data());
        get=get.substr(get.find_first_of("\r\n")+2);
        string key=get.substr(0,length);
        dt->args->push_back(key);
	}
	else if(format[0]=='M')
	{
		dt->type=REDIS_COMMAND_MULTI;
	}
	else if(format[0]=='D')
	{
		dt->type=REDIS_COMMAND_DEL;
        for(int i=0;i<argc-1;++i)
        {
			length=atoi(get.substr(1,get.find_first_of("\r\n")-1).data());
            get=get.substr(get.find_first_of("\r\n")+2);
            string key=get.substr(0,length);
            dt->args->push_back(key);
			get=get.substr(length+2);
        }
	}
	else if(format[0]=='S')
	{
		dt->type=REDIS_COMMAND_SET;
		for(int i=0;i<2;++i)
        {
			length=atoi(get.substr(1,get.find_first_of("\r\n")-1).data());
            get=get.substr(get.find_first_of("\r\n")+2);
            string key=get.substr(0,length);
            dt->args->push_back(key);
			get=get.substr(length+2);
        }
	}
	free(buf);
	length=sizeof(int);
	for(int i=0;i<dt->args->size();++i)
		length+=sizeof(int)+sizeof(char)*(*dt->args)[i].size();
	length+=sizeof(int);
	buf=(char*)malloc(length);
	int ptr=sizeof(int);
	buf[3]=(dt->type>>24)&0xFF;
	buf[2]=(dt->type>>16)&0xFF;
	buf[1]=(dt->type>>8)&0xFF;
	buf[0]=dt->type&0xFF;
	ptr+=sizeof(int);
	buf[7]=((dt->args->size())>>24)&0xFF;
	buf[6]=((dt->args->size())>>16)&0xFF;
	buf[5]=((dt->args->size())>>8)&0xFF;
	buf[4]=(dt->args->size())&0xFF;
	for(int i=0;i<dt->args->size();++i)
	{
		buf[ptr+3]=((*dt->args)[i].size()>>24)&0xFF;
		buf[ptr+2]=((*dt->args)[i].size()>>16)&0xFF;
		buf[ptr+1]=((*dt->args)[i].size()>>8)&0xFF;
		buf[ptr]=(*dt->args)[i].size()&0xFF;
		ptr+=sizeof(int);
		memcpy(buf+ptr,(*dt->args)[i].data(),(*dt->args)[i].size());
		ptr+=(*dt->args)[i].size();
	}
	//puts("length!");
	//cout<<length<<endl;
	//for(int i=0;i<length;++i)
	//{
	//	cout<<(int)buf[i]<<endl;
	//}
	//cout<<"format end"<<endl;
	if(send(c->fd,&length,sizeof(int),0)==-1)
    {
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
		delete dt->args;
        free(dt);
		free(buf);
        return r;
    }
    if(send(c->fd,buf,length,0)==-1)
    {
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
		delete dt->args;
        free(dt);
		free(buf);
        return r;
    }
	free(buf);
	delete dt->args;
	free(dt);
	if(recv(c->fd,&length,sizeof(int),0)==-1)
	{
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
        return r;
    };
	buf=(char*)malloc(length);
    if(recv(c->fd,buf,length,0)==-1)
    {
        c->err=1;
        strcpy(c->errstr,strerror(errno));
        r->type=REDIS_REPLY_ERROR;
        return r;
    };
	ptr=sizeof(redisReply);
	memcpy(r,buf,sizeof(redisReply));
	r->str=(char*)malloc(r->len);
	memcpy(r->str,buf+ptr,r->len);
	ptr+=r->len;
	r->element=(redisReply**)malloc(r->elements*sizeof(redisReply));
	for(int i=0;i<r->elements;++i)
	{
		memcpy(r->element[i],buf+ptr,sizeof(redisReply));
		ptr+=sizeof(redisReply);
		r->element[i]->str=(char*)malloc(r->element[i]->len);
		memcpy(r->element[i]->str,buf+ptr,r->element[i]->len);
		ptr+=r->element[i]->len;
	}
	//cout<<(r->str)<<endl;
    return r;
}

void freeReplyObject(void *reply)
{
    if(reply==NULL)
        return;
    free(reply);
    return;
}

void redisFree(redisContext *c)
{
    if(c->fd>0)
        close(c->fd);
    free(c);
    return;
}
