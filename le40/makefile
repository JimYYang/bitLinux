cc=g++

.PHONY:all
all:udpClient udpServer

udpClient:udpClient.cc
	$(cc) -o $@ $^ -std=c++17
udpServer:udpServer.cc
	$(cc) -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f udpClient udpServer
