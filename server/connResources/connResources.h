#ifndef _CONN_RESOURCES_H_
#define _CONN_RESOURCES_H_

#include <unordered_map>
#include <fstream>

/// @brief 用户系统资源
class ConnResources
{
public:
    ConnResources();
    ConnResources(std::string _userId);
    ~ConnResources();

    std::string getUserId();
    void setUserId(std::string _userId);

    std::ofstream* getUploadStream();
    bool setUploadStream(std::ofstream* of);

    std::string getFilePath();
    void setFilePath(std::string _filepath);

private:
    std::string userId;
    std::ofstream* uploadTo;
    std::ifstream* downloadFrom;
    std::string filepath;
};

#endif