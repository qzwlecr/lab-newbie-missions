main:server client

server:redis.o networking.o databases.o 
	g++ -o server redis.o networking.o databases.o

client:redis-cli.o hiredis.o
	g++ -o client redis-cli.o hiredis.o

hiredis.o:hiredis.cpp
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
	rm *.o server

