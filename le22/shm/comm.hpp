#ifndef _COMM_HPP_
#define _COMM_HPP_

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define PATHNAME "."
#define PROJ_ID 0x66
#define MAX_SIZE 4096

key_t getKey()
{
    key_t k = ftok(PATHNAME, PROJ_ID);
    if (k < 0)
    {
        std::cerr << errno << ":" << strerror(errno) << std::endl;
        exit(1);
    }
    return k;
}

int getShmHelper(key_t k, int flags)
{
    int shmid = shmget(k, MAX_SIZE, flags);
    if (shmid < 0)
    {
        std::cerr << errno << ":" << strerror(errno) << std::endl;
        exit(2);
    }
    return shmid;
}

int createShm(key_t k )
{
    return getShmHelper(k, IPC_CREAT | IPC_EXCL | 0600); // 存在会出错
}

int getShm(key_t k)
{
    return getShmHelper(k, IPC_CREAT);
}

void delShm(int shmid)
{
    if (shmctl(shmid, IPC_RMID, nullptr) == -1)
    {
        std::cerr << errno << ":" << strerror(errno) << std::endl;
        exit(3);
    }
}

void *attachShm(int shmid)
{
    void *mem = shmat(shmid, nullptr, 0);
    if ((long long)mem == -1l)
    {
        std::cerr << errno << ":" << strerror(errno) << std::endl;
        exit(4);
    }
    return mem;
}

void detachShm(void *start)
{
    if (shmdt(start) == -1)
    {
        std::cerr << errno << ":" << strerror(errno) << std::endl;
        exit(5);
    }
}

#endif