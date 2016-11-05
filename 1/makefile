all : index search

index:index.c
	gcc index.c -lmagic -lhiredis -o index 

search:search.c
	gcc search.c -lhiredis -o search

	
clean:
	rm index search
