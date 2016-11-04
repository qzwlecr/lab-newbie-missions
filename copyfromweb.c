//cLocal points to the local redis

llCursor=0;

done=false;

while(!done)

{

    cmd=”zscan “+setName+” %lld count 10″;
    reply = (redisReply *)redisCommand(cLocal, cmd.c_str(), llCursor);
    if(reply == NULL)
    {
        cout << “scan ” << setName << “failed, error is: ” << cLocal->errstr;
        redisFree(cLocal);
        cLocal = NULL;
        break;
    }

    if(reply->type == REDIS_REPLY_ARRAY)
    {
        if(reply->elements == 0)
        {
            done=true;
            cout << “get 0 msg from ” << setName;
        }
        else
        {
            llCursor=boost::lexical_cast<long long>(reply->element[0]->str);  //reply->element[0] contains cursor

            redisReply ** siteCounters=reply->element[1]->element;               //reply->element[1] contains elements array:value1,score1,value2,score2,…
            for(size_t i=0; i<reply->element[1]->elements; i++)
            {
                string elem = siteCounters[i++]->str;
                string score = siteCounters[i]->str;

            }
                if(llCursor == 0)  //scan over
                {
                done=true;
                }
            }
    }
    else
    {
        done=true;
    }
    freeReplyObject(reply);

}