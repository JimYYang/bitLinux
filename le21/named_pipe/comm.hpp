#pragma once

#include <iostream>
#include <string>
#include <cerrno>
#include <cassert>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define NAMED_PIPE "/tmp/mypipe.106"

bool createFifo(const std::string &path)
{
    umask(0);
    int n = mkfifo(path.c_str(), 0600);
    if (n == 0) return true;
    else
    {
        std::cout << "errno: " << errno << " err string: " << strerror(errno) << std::endl;
        return false;
    }
}

void removeFifo(const std::string &path)
{
    int n = unlink(path.c_str());
    assert(n == 0); // debug release没有
    (void)n;
}