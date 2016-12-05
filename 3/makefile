server:redis.o networking.o databases.o 
	g++ -o server redis.o networking.o databases.o

redis.o:redis.cpp
	g++ -c redis.cpp

networking.o:networking.cpp
	g++ -c networking.cpp

databases.o:databases.cpp
	g++ -c databases.cpp

clean:
	rm *.o server

