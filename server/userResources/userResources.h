#ifndef _USER_RESOURCES_H_
#define _USER_RESOURCES_H_

#include <unordered_map>
#include <fstream>

/// @brief 用户系统资源
class UserResources
{
public:
    UserResources();
    UserResources(std::string _userId);
    ~UserResources();

    std::string getUserId();
    void setUserId(std::string _userId);

    std::ofstream* getOfstream(std::string path);
    bool addToUploadList(std::string path);

private:
    std::string userId;
    std::unordered_map<std::string, std::ofstream*> uploadlist;
};

#endif