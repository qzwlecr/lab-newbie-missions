main:server libhiredis.a

server:redis.o networking.o databases.o
	g++ -o server redis.o networking.o databases.o

libhiredis.a: hiredis.o sds.o fakeredis.o
	ar rcs libhiredis.a  hiredis.o sds.o fakeredis.o

sds.o:sds.cpp sds.h
	g++ -c sds.cpp

fakeredis.o:fakeredis.cpp fakeredis.h
	g++ -c fakeredis.cpp

hiredis.o:hiredis.cpp
	g++ -c hiredis.cpp

redis.o:redis.cpp
	g++ -c redis.cpp

networking.o:networking.cpp
	g++ -c networking.cpp 

databases.o:databases.cpp 
	g++ -c databases.cpp

clean:
	rm *.o *.a server

