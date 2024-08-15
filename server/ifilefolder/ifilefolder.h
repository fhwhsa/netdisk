#ifndef _IFILEFOLDER_H_
#define _IFILEFOLDER_H_

#include <string>
#include <vector>

/// @brief 文件或文件夹操作接口
class IFileFolder
{

public:

    /// @brief 获取目录下所有文件名
    /// @param path 目录路径
    /// @param res 处理结果，成功为true
    /// @param statusCode 处理结果状态码
    /// @return 成功返回一个vector，每个元素是一对值，int为0代表目录，1代表文件，string代表文件名
    static std::vector<std::pair<int, std::string>> getFolderContent(std::string path, bool& res, int& statusCode);

    /// @brief 在指定路径下创建指定名称的文件夹
    /// @param path 要创建的文件夹所在路径
    /// @param name 文件夹名称
    /// @param statusCode 处理结果状态码
    /// @return 操作结果，成功返回true
    static bool createFolder(std::string path, std::string name, int& statusCode);

    /// @brief 删除指定路径的文件/文件夹
    /// @param path 目标文件/文件夹路径
    /// @param statusCode 处理结果状态码
    /// @return 操作结果，成功返回true
    static bool deleteFileOrFolder(std::string path, int& statusCode);

    /// @brief 重命名
    /// @param path 文件/文件夹路径
    /// @param newName 新名称
    /// @param statusCode 处理结果状态码
    /// @return 操作结果，成功返回true
    static bool renameFileOrFolder(std::string path, std::string newName, int& statusCode);

    /// @brief 创建文件
    /// @param path 文件路径
    /// @param statusCode 处理结果状态码
    /// @return 成功返回打开的文件标识符，否则返回空指针
    static int createFile(std::string path, int& statusCode);

private:
    IFileFolder();
    IFileFolder(const IFileFolder& obj);
    IFileFolder& operator=(const IFileFolder& obj);
};

#endif
