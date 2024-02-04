#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include "log.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NUM 1024

class TcpClient
{
public:
    TcpClient(const std::string &server_ip, const uint16_t &server_port)
    : _sock(-1), _server_ip(server_ip), _server_port(server_port)
    {}

    void initClient()
    {
        // 1. 创建socket
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_sock < 0)
        {
            std::cerr << "socket create error" << std::endl;
            exit(2);
        }

        // 2. tcp客户端需要bind  但是不需要显示bind 这里尤其是client_port要OS自动分配
        // 3. 不要listen
        // 4. 不要accept
        // 5. 要发起连接
    }

    void start()
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_family = AF_INET;
        server.sin_port = htons(_server_port);
        server.sin_addr.s_addr = inet_addr(_server_ip.c_str());

        if (connect(_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
        {
            std::cerr << "socket connet error";
            exit(3);
        }
        else
        {
            std::string msg;
            while (true)
            {
                std::cout << "Enter# ";
                std::getline(std::cin, msg);
                write(_sock, msg.c_str(), msg.size());

                char buffer[NUM];
                int n = read(_sock, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    buffer[n] = 0;
                    std::cout << "server 回显# " << buffer << std::endl;
                }
                else
                {
                    break;
                }
            }
        }
    }

    ~TcpClient()
    {
        if (_sock >= 0) close(_sock);
    }
private:
    int _sock;
    std::string _server_ip;
    uint16_t _server_port;
};