cc=g++

.PHONY:all
all:tcpServer tcpClient

tcpServer:tcpServer.cc
	$(cc) -o $@ $^ -lpthread -std=c++11
tcpClient:tcpClient.cc
	$(cc) -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcpServer tcpClient