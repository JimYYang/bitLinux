#pragma warning(disable:4996)

#include <iostream>
#include <string>
#include <cstring>
#include <WinSock2.h>

#define NUM 1024

#pragma comment(lib, "ws2_32.lib")

using namespace std;

uint16_t server_port = 8080;
string server_ip = "47.100.15.220";

int main()
{
	WSAData wsd;

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup Error = " << WSAGetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "WSAStartup Success" << endl;
	}

	SOCKET csock = socket(AF_INET, SOCK_DGRAM, 0);

	if (csock == SOCKET_ERROR)
	{
		cout << "socket error = " << WSAGetLastError() << endl;
		return 1;
	}
	else
	{
		cout << "socket Success" << endl;
	}

	struct sockaddr_in server;
	memset(&server, 0, sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);
	server.sin_addr.s_addr = inet_addr(server_ip.c_str());

	char  inbuffer[NUM];
	string line;
	while (true)
	{
		cout << "please Enter# ";
		getline(cin, line);
		int n = sendto(csock, line.c_str(), line.size(), 0, (struct sockaddr*)&server, sizeof(server));
		if (n < 0)
		{
			cerr << "sendto Error" << endl;
			break;
		}
		inbuffer[0] = 0;
		struct sockaddr_in peer;
		int peerlen = sizeof(peer);

		n = recvfrom(csock, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr*)& peer, &peerlen);
		if (n > 0)
		{
			inbuffer[n] = 0;
			cout << "server 返回的消息: " << inbuffer << endl;
		}
		else break;

	}

	closesocket(csock);
	WSACleanup();
}
