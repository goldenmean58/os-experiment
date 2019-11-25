.PHONY: all clean

main: pipe A B server client main.cpp
	@clang++ main.cpp -c -o main.o
	@clang++ A.o B.o server.o client.o pipe.o main.o -o main

pipe: pipe.cpp main.h
	@clang++ pipe.cpp -c -o pipe.o

A: A.cpp main.h
	@clang++ A.cpp -c -o A.o

B: B.cpp main.h
	@clang++ B.cpp -c -o B.o

server: server.cpp main.h
	@clang++ server.cpp -c -o server.o

client: client.cpp main.h
	@clang++ client.cpp -c -o client.o

clean:
	@rm pipe.o A.o B.o server.o client.o main.o main
