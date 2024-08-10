#include "connResources.h"

ConnResources::ConnResources()
{
}

ConnResources::ConnResources(std::string _userId) : userId(_userId)
{
}

std::string ConnResources::getUserId()
{
    return userId;
}

ConnResources::~ConnResources()
{
    if (nullptr != uploadTo)
    {
        uploadTo->close();
        delete uploadTo;
        uploadTo = nullptr;
    }
    if (nullptr != downloadFrom)
    {
        downloadFrom->close();
        delete downloadFrom;
        downloadFrom = nullptr;
    }
}

void ConnResources::setUserId(std::string _userId)
{
    userId = _userId;
}

bool ConnResources::setUploadStream(std::ofstream *of)
{
    if (nullptr == of || !of->is_open())
        return false;
    uploadTo = of;
    return true;
}

std::ofstream *ConnResources::getUploadStream()
{
    return uploadTo;
}

std::string ConnResources::getFilePath()
{
    return filepath;
}

void ConnResources::setFilePath(std::string _filepath)
{
    filepath = _filepath;
}
