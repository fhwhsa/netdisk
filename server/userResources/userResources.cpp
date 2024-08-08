#include "userResources.h"

UserResources::UserResources()
{
}

UserResources::UserResources(std::string _userId) : userId(_userId)
{
}

std::string UserResources::getUserId()
{
    return userId;
}

UserResources::~UserResources()
{
    for (auto it = uploadlist.begin(); it != uploadlist.end(); ++it)
    {
        if (nullptr != it->second)
        {
            it->second->close();
            delete it->second;
            it->second = nullptr;
        }
    }
}

void UserResources::setUserId(std::string _userId)
{
    userId = _userId;
}

std::ofstream *UserResources::getOfstream(std::string path)
{
    if (uploadlist.find(path) == uploadlist.end())
        return nullptr;
    return uploadlist[path];
}

bool UserResources::addToUploadList(std::string path)
{
    using namespace std;
    ofstream* of = new ofstream(path, std::ios::binary);
    if (!of->is_open())
        return false;
    uploadlist[path] = of; 
    return true;
}
