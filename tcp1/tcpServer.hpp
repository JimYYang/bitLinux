#pragma once


#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include "log.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <pthread.h>


namespace server
{
    enum {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR
    };
    static const uint16_t gport = 8080;
    static const int gbacklog = 5;

    class TcpServer;
    class ThreadData
    {
    public:
        ThreadData(TcpServer *self, int sock)
        : _self(self), _sock(sock)
        {}
    public:
        TcpServer *_self;
        int _sock;
    };

    class TcpServer
    {
    public:
        TcpServer(const uint16_t &port = gport)
        : _listen_sock(-1), _port(gport)
        {}

        void initServer()
        {
            // 1. 创建套接字对象
            _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listen_sock < 0)
            {
                logMessage(FATAL, "create socket error");
                exit(SOCKET_ERR);
            }
            logMessage(NORMAL, "create socket success");

            // 2. bind网络信息
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = INADDR_ANY;
            if(bind(_listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
            {
                logMessage(FATAL, "bind socket error");
                exit(BIND_ERR);
            }
            logMessage(NORMAL, "bind socket success");

            // 3. 设置socket为监听状态
            // listen的作用是获取连接
            if (listen(_listen_sock, gbacklog) < 0)
            {
                logMessage(FATAL, "listen socket error");
                exit(LISTEN_ERR);
            }
            logMessage(NORMAL, "listen socket success");
        }

        static void *threadRoutine(void *args)
        {
            pthread_detach(pthread_self());
            ThreadData *td = static_cast<ThreadData*>(args);
            td->_self->serviceIO(td->_sock);
            close(td->_sock);
            delete td;
            td = nullptr;
            return nullptr;
        }

        void start()
        {
            // version 2 
            // signal(SIGCHLD, SIG_IGN);
            for (;;)
            {
                // 4. server 获取新连接
                // 这个套接字是和client进行通信的fd
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                int sock = accept(_listen_sock, (struct sockaddr*)&peer, &len);
                if (sock < 0)
                {
                    logMessage(ERROR, "accept error, next");
                    continue;
                }
                logMessage(NORMAL, "accept a new link success");
                std::cout << "new sock: " << sock << std::endl;

                // 5. 这里就是一个sock  未来通信就用这个sock 面向字节流的  后续都是文件操作
                // serviceIO(sock);
                // close(sock); // 对于一个使用完毕的sock  要关闭 不然会导致文件描述符泄漏

                // version 2 多进程版
                // pid_t id = fork();
                // if (id == 0) // child
                // {
                //     close(_listen_sock);

                //     // if (fork() > 0) exit(0);

                //     serviceIO(sock);
                //     close(sock); // 对于一个使用完毕的sock  要关闭 不然会导致文件描述符泄漏

                //     exit(0);
                // }

                // close(sock);

                // father
                // pid_t ret = waitpid(id, nullptr, 0); // 0代表阻塞等待
                // if (ret > 0)
                // {
                //     std::cout << "wait success: " << ret << std::endl;
                // }


                // version 3 多线程版
                // pthread_t tid;
                // ThreadData *td = new ThreadData(this, sock); 
                // pthread_create(&tid, nullptr, threadRoutine, td);

                // version 4 线程池
                
            }
        }

        void serviceIO(int sock)
        {
            while (true)
            {
                char buffer[1024];
                ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    buffer[n] = 0;
                    std::cout << "recv message: " << buffer << std::endl;

                    std::string outbuffer = buffer;
                    outbuffer += "server[echo]";

                    write(sock, outbuffer.c_str(), outbuffer.size());
                }
                else if (!n)
                {
                    // 代表客户端退出
                    logMessage(NORMAL, "client quit, me too!");
                    break;
                }
            }
        }
        ~TcpServer()
        {}
    
    private:
        int _listen_sock; // 不是用来进行数据通信的 只是用来监听新连接到来的 获取新连接的
        uint16_t _port;
    };
}