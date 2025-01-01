#ifndef _IFILEFOLDER_H_
#define _IFILEFOLDER_H_

#include "../statusCode/statusCode.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

/// @brief 文件信息
struct FileInfo
{
    /// @brief 文件类型，0代表文件夹，1代表文件 
    int type; 
    /// @brief 文件/文件夹名
    std::string fileName;
    /// @brief 文件/文件夹大小
    uintmax_t fileSize;

    FileInfo();
    FileInfo(int _type, std::string _fileName, uintmax_t _fileSize) : type(_type), fileName(_fileName), fileSize(_fileSize) {}; 
    bool operator< (const FileInfo& f) const 
    {
        if (type != f.type)
            return type < f.type;
        return fileName < f.fileName;
    }
};

/// @brief 文件或文件夹操作API
class IFileFolder
{

public:

    /// @brief 获取目录下所有文件名
    /// @param path 目录路径（网盘路径）
    /// @param res 处理结果，成功为true
    /// @param statusCode 处理结果状态码
    /// @return 返回目录下所有文件夹/文件信息
    static std::vector<FileInfo> getFolderContent(std::string path, bool& res, int& statusCode);

    /// @brief 在指定路径下创建指定名称的文件夹
    /// @param path 要创建的文件夹所在路径（网盘路径）
    /// @param name 文件夹名称
    /// @param statusCode 处理结果状态码
    /// @return 操作结果，成功返回true
    static bool createFolder(std::string path, std::string name, int& statusCode);

    /// @brief 删除指定路径的文件/文件夹
    /// @param path 目标文件/文件夹路径（网盘路径）
    /// @param statusCode 处理结果状态码
    /// @return 操作结果，成功返回true
    static bool deleteFileOrFolder(std::string path, int& statusCode);

    /// @brief 重命名
    /// @param path 文件/文件夹路径（网盘路径）
    /// @param newName 新名称
    /// @param statusCode 处理结果状态码
    /// @return 操作结果，成功返回true
    static bool renameFileOrFolder(std::string path, std::string newName, int& statusCode);

    /// @brief 创建文件
    /// @param path 文件路径（网盘路径）
    /// @param statusCode 处理结果状态码
    /// @return 成功返回打开的文件标识符，否则返回-1
    static int createFile(std::string path, int& statusCode);

    /// @brief 打开文件（只读）
    /// @param path 文件路径（网盘路径）
    /// @param offset 指定位置
    /// @param statusCode 处理结果状态码
    /// @return 
    static int openFile(std::string path, off_t offset, int& statusCode);

    /// @brief 打开文件（追加，只写）
    /// @param path 文件路径（网盘路径）
    /// @param statusCode 处理结果状态码
    /// @param fileSize 这是一个传出参数，标识文件当前大小（byte）
    /// @return 成功返回打开的文件标识符，否则返回-1
    static int openFile(std::string path, int& statusCode, long* fileSize = nullptr);

    /// @brief 获取文件大小（byte）
    /// @param path 文件路径（网盘路径）
    /// @return 成功返回指定路径的文件大小，否则返回-1
    static long getFileSize(std::string path);

private:
    IFileFolder();
    IFileFolder(const IFileFolder& obj);
    IFileFolder& operator=(const IFileFolder& obj);
};

#endif
