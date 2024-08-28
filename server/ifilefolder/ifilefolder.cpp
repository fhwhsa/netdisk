#include "ifilefolder.h"
#include "../statusCode/statusCode.h"

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

const std::string basePath = "./virtualDisks";

std::vector<FileInfo> IFileFolder::getFolderContent(std::string path, bool& res, int& statusCode)
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

    vector<FileInfo> v;
    for (const auto& it : directory_iterator(path))
    {
        bool isDir = it.is_directory();
        FileInfo fi((isDir ? 0 : 1), it.path().filename(), isDir ? -1 : it.file_size());
        v.push_back(move(fi));
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
    newName = path.substr(0, path.rfind('/') + 1) + newName;

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
        {
            statusCode = FILEOPENFAILURE;
            perror("open");
        }
        return fd;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        statusCode = EXCEPTION;
    }
    return -1;
}

int IFileFolder::openFile(std::string path, off_t offset, int &statusCode)
{
    using namespace std;
    using namespace filesystem;

    path = basePath + path;

    if (!exists(path))
    {
        statusCode = FILEORFOLDERNOTEXIST;
        return -1;
    }
    try
    {
        statusCode = SUCCESS;
        int fd = open(path.c_str(), O_RDONLY);
        if (-1 == fd)
        {
            statusCode = FILEOPENFAILURE;
            perror("open");
        }
        if (-1 == lseek(fd, offset, SEEK_SET))
        {
            statusCode = FILEOPENFAILURE;
            perror("lseek"); 
        }
        return fd;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        statusCode = EXCEPTION;
    }
    return -1;
}

int IFileFolder::openFile(std::string path, int &statusCode,  long* fileSize)
{
    using namespace std;
    using namespace filesystem;

    path = basePath + path;

    if (!exists(path))
    {
        statusCode = FILEORFOLDERNOTEXIST;
        return -1;
    }
    try
    {
        statusCode = SUCCESS;
        int fd = open(path.c_str(), O_WRONLY | O_APPEND);
        if (-1 == fd)
        {
            statusCode = FILEOPENFAILURE;
            perror("open");
            return -1;
        }
        if (nullptr != fileSize)
        {
            struct stat st;
            if (-1 == fstat(fd, &st))
            {
                statusCode = GETFILESTATFAILURE;
                perror("fstat");
                return -1;
            }
            (*fileSize) = st.st_size;
        }
        return fd;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        statusCode = EXCEPTION;
    }
    return -1;
}

long IFileFolder::getFileSize(std::string path)
{
    path = basePath + path;
    struct stat filestat;
    if (0 != stat(path.c_str(), &filestat))
        return -1;
    return static_cast<long>(filestat.st_size);
}
