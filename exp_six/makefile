.PHONY: all clean

all: pipe A B server client

pipe: pipe.cpp
	@clang++ pipe.cpp -o pipe

A: A.cpp
	@clang++ A.cpp -o A

B: B.cpp
	@clang++ B.cpp -o B

server: server.cpp
	@clang++ server.cpp -o server

client: client.cpp
	@clang++ client.cpp -o client

clean:
	@rm pipe A B server client
