#include "ifilefolder.h"

#include <iostream>
#include <filesystem>
#include <algorithm>

const std::string basePath = "userresources";

std::vector<std::pair<int, std::string>> IFileFolder::getFolderContent(std::string path, bool& res)
{
    using namespace std;
    using namespace filesystem;

    path = basePath + path;

    if (!exists(path) || !is_directory(path))
    {
        res = false;
        return {};
    }

    vector<pair<int, string>> v;
    for (const auto& it : directory_iterator(path))
    {
        v.emplace_back(it.is_directory() ? 0 : 1, it.path().filename());
    }

    res = true;
    sort(v.begin(), v.end());
    return v;
}

bool IFileFolder::createFolder(std::string path, std::string name)
{
    using namespace std;
    using namespace filesystem;
    
    path = basePath + path;
    
    try
    {
        return create_directory(path + "/" + name);
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << '\n';   
    }
    return false;
}

bool IFileFolder::deleteFileOrFolder(std::string path)
{
    using namespace std;
    using namespace filesystem;

    path = basePath + path;

    if (!exists(path))
        return false;

    if (is_directory(path))
    {
        try
        {
            return remove_all(path) > 0;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return false;    
    }
    else 
    {
        try
        {
            return remove(path);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return false;
    }
}

bool IFileFolder::renameFileOrFolder(std::string path, std::string newName)
{
    using namespace std;
    using namespace filesystem;
    
    path = basePath + path;

    if (!exists(path))
        return false;

    int t = path.rfind('/');
    if (-1 != t)
        newName = path.substr(0, t + 1) + newName;

    try
    {
        rename(path, newName);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
    return exists(newName) && !exists(path);
}
