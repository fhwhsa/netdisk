#include "connResources.h"

ConnResources::ConnResources()
{
    fd = -1;
    userId = "null";
}

ConnResources::ConnResources(std::string _userId) : userId(_userId)
{
    fd = -1;
}

std::string ConnResources::getUserId()
{
    return userId;
}

ConnResources::~ConnResources()
{
    if (-1 != fd)
        close(fd);
}

void ConnResources::setUserId(std::string _userId)
{
    userId = _userId;
}

std::string ConnResources::getFilePath()
{
    return filepath;
}

void ConnResources::setFilePath(std::string _filepath)
{
    filepath = _filepath;
}

void ConnResources::setFd(int _fd)
{
    fd = _fd;
}

int ConnResources::getFd()
{
    return fd;
}
