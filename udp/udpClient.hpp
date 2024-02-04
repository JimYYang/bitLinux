#pragma once


#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/in.h>
#include <pthread.h>


namespace Client
{
    using namespace std;
    class udpClient
    {
        public:
        udpClient(const string &serverip, const uint16_t &serverport)
        : _serverip(serverip), _serverport(serverport), _sockfd(-1), _quit(false)
        {}

        void initClient()
        {
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (_sockfd == -1)
            {
                cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
                exit(2);
            }
            cout << "socket success: " << " : " << _sockfd << endl;
        }

        static void *readMessage(void *args)
        {
            int sockfd = *(static_cast<int*>(args));
            pthread_detach(pthread_self());

            while(true)
            {
                char buffer[1024];
                struct sockaddr_in temp;
                socklen_t temp_len = sizeof(temp);
                size_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&temp, &temp_len);
                if (n >= 0)
                {
                    buffer[n] = 0;
                }
                cout << buffer << endl;
            }
            return nullptr;
        }
        void run()
        {
            pthread_create(&_reader, nullptr, readMessage, (void*)&_sockfd);
            struct sockaddr_in server;
            memset(&server, 0, sizeof server);
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(_serverip.c_str());
            server.sin_port = htons(_serverport);

            string message;
            char line[1024];
            while (!_quit)
            {
                // 输出重定向 一个输出到管道 一个输出到屏幕  输入没有做重定向
                // cerr << "# ";
                // cin >> message;
                fprintf(stderr, "Enter# ");
                fflush(stderr);

                // fgets会获取回车
                fgets(line, sizeof(line), stdin);
                line[strlen(line) - 1] = 0;
                message = line;

                sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));

                // cout << "server tansform " << buffer << endl;
            }
        }
        ~udpClient()
        {}
    private:
        int _sockfd;
        string _serverip;
        uint16_t _serverport; 
        bool _quit;
        pthread_t _reader;
    };
}
