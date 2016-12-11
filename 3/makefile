main:server client

server:redis.o networking.o databases.o
	g++ -o server redis.o networking.o databases.o

client:redis-cli.o hiredis.o sds.o fakeredis.o
	g++ -o client redis-cli.o hiredis.o sds.o fakeredis.o

sds.o:sds.cpp sds.h
	g++ -c sds.cpp

fakeredis.o:fakeredis.cpp fakeredis.h
	g++ -c fakeredis.cpp

hiredis.o:hiredis.cpp hiredis.h
	g++ -c hiredis.cpp

redis-cli.o:redis-cli.cpp
	g++ -c redis-cli.cpp

redis.o:redis.cpp
	g++ -c redis.cpp

networking.o:networking.cpp
	g++ -c networking.cpp 

databases.o:databases.cpp 
	g++ -c databases.cpp

clean:
	rm *.o server client

