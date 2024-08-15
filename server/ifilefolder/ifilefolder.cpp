#include "ifilefolder.h"
#include "../statusCode/statusCode.h"

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

const std::string basePath = "./virtualDisks";

std::vector<std::pair<int, std::string>> IFileFolder::getFolderContent(std::string path, bool& res, int& statusCode)
{
    using namespace std;
    using namespace filesystem;

    path = basePath + path;

    if (!exists(path)) 
    {
        res = false;
        statusCode = FILEORFOLDERNOTEXIST;
        return {};
    }
    if (!is_directory(path))
    {
        res = false;
        statusCode = ISNOTFOLDER;
        return {};
    }

    vector<pair<int, string>> v;
    for (const auto& it : directory_iterator(path))
    {
        v.emplace_back(it.is_directory() ? 0 : 1, it.path().filename());
    }

    res = true;
    sort(v.begin(), v.end());
    statusCode = SUCCESS;
    return v;
}

bool IFileFolder::createFolder(std::string path, std::string name, int& statusCode)
{
    using namespace std;
    using namespace filesystem;
    
    path = basePath + path + "/" + name;
    if (exists(path))
    {
        statusCode = FOLDEREXIST;
        return false;
    }
    
    try
    {
        statusCode = SUCCESS;
        return create_directory(path);
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << '\n';  
        statusCode = EXCEPTION; 
    }
    return false;
}

bool IFileFolder::deleteFileOrFolder(std::string path, int& statusCode)
{
    using namespace std;
    using namespace filesystem;

    path = basePath + path;

    if (!exists(path))
    {
        statusCode = FILEORFOLDERNOTEXIST;
        return false;
    }

    if (is_directory(path))
    {
        try
        {
            statusCode = SUCCESS;
            return remove_all(path) > 0;
        }
        catch(const std::exception& e)
        {
            statusCode = EXCEPTION;
            std::cerr << e.what() << '\n';
        }
        return false;    
    }
    else 
    {
        try
        {
            statusCode = SUCCESS;
            return remove(path);
        }
        catch(const std::exception& e)
        {
            statusCode = EXCEPTION;
            std::cerr << e.what() << '\n';
        }
        return false;
    }
}

bool IFileFolder::renameFileOrFolder(std::string path, std::string newName, int& statusCode)
{
    using namespace std;
    using namespace filesystem;
    
    path = basePath + path;
    newName = basePath + newName;

    if (!exists(path))
    {
        statusCode = FILEORFOLDERNOTEXIST;
        return false;
    }

    try
    {
        rename(path, newName);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        statusCode = EXCEPTION;
        return false;
    }
    
    if (exists(newName) && !exists(path))
    {
        statusCode = SUCCESS;
        return true;
    }
    statusCode = FAILURE;
    return false;
}

int IFileFolder::createFile(std::string path, int &statusCode)
{
    using namespace std;
    using namespace filesystem;

    path = basePath + path;

    if (exists(path))
    {
        statusCode = FILEEXIST;
        return -1;
    }
    try
    {
        statusCode = SUCCESS;
        int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
        if (-1 == fd)
            perror("open");
        return fd;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        statusCode = EXCEPTION;
    }
    return -1;
}
