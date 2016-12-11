#include <iostream>
#include <cmath>
#include <ctime>
#include "hiredis.h"
using namespace std;
#define TEST_NUM 10000000
#define EPSILON 1e-5
redisContext* conn;
redisReply* rep;
char* vrep;
clock_t t;
double totTime;
struct testInfo{
    double doubleInfo;
    int intInfo;
} info[TEST_NUM];

inline bool testDouble(double a,double b){
    return(abs(a-b)<EPSILON);
}
int main() {
    conn=redisConnect("127.0.0.1",6379);
    if (conn == NULL || conn->err) {
        if (conn) {
            printf("Error: %s\n", conn->errstr);
        } else {
            printf("Can't allocate redis context\n");
        }
        return(-1);
    }
    for(int i=0;i<TEST_NUM;++i){
        info[i].intInfo=i;
        info[i].doubleInfo=sin(i);
    }

    t=clock();
    for(int i=0;i<TEST_NUM;++i){
        vrep=(char*)redisCommand(conn,"SET k%d %b",i,&info[i],sizeof(testInfo));
        if(!vrep||vrep==(char*)REDIS_ERR){
            cout<<conn->errstr<<endl;
            return(-1);
        }
        freeReplyObject((redisReply*)vrep);
    }
    t=clock()-t;
    totTime=((double)t)/CLOCKS_PER_SEC;
    cout<<"Insertion test complete in "<<totTime<<" seconds"<<endl;
    testInfo* repInfo;
    t=clock();
    for(int i=0;i<TEST_NUM;++i){
        rep=(redisReply*)redisCommand(conn,"GET k%d",i);
        if(!rep||rep==(redisReply*)REDIS_ERR){
            cout<<conn->errstr<<endl;
            return(-1);
        }
        if(rep->type!=REDIS_REPLY_STRING){
            cout<<"Return value has a wrong type!"<<endl;
            return(-1);
        }

        repInfo=(testInfo*)rep->str;

        if(i!=repInfo->intInfo||!testDouble(info[i].doubleInfo,repInfo->doubleInfo)){
            cout<<"Returned wrong data"<<endl;
            cout<<"Original data:"<<i<<","<<info[i].doubleInfo<<endl;
            cout<<"Returned data:"<<repInfo->intInfo<<","<<repInfo->doubleInfo<<endl;
            return(-1);
        }
        freeReplyObject(rep);
    }
    t=clock()-t;
    totTime=((double)t)/CLOCKS_PER_SEC;
    cout<<"Query test complete in "<<totTime<<" seconds"<<endl;

    t=clock();
    for(int i=(TEST_NUM/2)-1;i>=0;--i){
        rep=(redisReply*)redisCommand(conn,"DEL k%d",i*2);
        if(!rep||rep==(redisReply*)REDIS_ERR){
            cout<<conn->errstr<<endl;
            return(-1);
        }
        freeReplyObject(rep);
    }
    rep=(redisReply*)redisCommand(conn,"GET k5");
    if(!rep) {
        cout << conn->errstr << endl;
        return (-1);
    }
    if(rep->type!=REDIS_REPLY_STRING){
        cout<<"Deleted wrong key!"<<endl;
        return(-1);
    }
    freeReplyObject(rep);

    rep=(redisReply*)redisCommand(conn,"GET k4");
    if(!rep) {
        cout << conn->errstr << endl;
        return (-1);
    }
    if(rep->type!=REDIS_REPLY_NIL){
        cout<<"Key not deleted!"<<endl;
        return(-1);
    }
    freeReplyObject(rep);

    for(int i=(TEST_NUM/2)-1;i>=0;--i){
        rep=(redisReply*)redisCommand(conn,"DEL k%d",i*2-1);
        if(!rep||rep==(redisReply*)REDIS_ERR){
            cout<<conn->errstr<<endl;
            return(-1);
        }
        freeReplyObject(rep);
    }
    t=clock()-t;
    totTime=((double)t)/CLOCKS_PER_SEC;
    cout<<"Deletion test complete in "<<totTime<<" seconds"<<endl;

    redisFree(conn);
    return 0;
}
