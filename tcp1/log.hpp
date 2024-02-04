#pragma once

#include <iostream>
#include <vector>

#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERROR 3
#define FATAL 4

void logMessage(int level, const std::string &message)
{
    std::cout << message << std::endl;
}