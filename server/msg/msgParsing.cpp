#include "msgParsing.h"

std::vector<std::string> MsgParsing::split_string(const std::string &str, std::string reg)
{
    using namespace std;
    vector<string> tokens;
    regex pattern(reg);
    sregex_token_iterator iter(str.begin(), str.end(), pattern, -1);
    sregex_token_iterator end;
    for (; iter != end; iter++)
        tokens.push_back(*iter);
    return tokens; 
}

MsgUnit *MsgParsing::loginRespond(const MsgUnit *munit, ConnResources& ur)
{
    using namespace std;
    string msg((char*)munit->msg);
    vector<string> tokens = split_string(msg, "\r\n");

    if (tokens.size() < 2)
        return nullptr;
    string email = tokens[0].substr(6);
    string passwd = tokens[1].substr(7);

    // cout << email << " " << passwd << "\n" << flush;
    
    MsgUnit* respond = nullptr;
    int id, statusCode;
    string content;
    // 身份验证成功
    if (-1 != (id = IDatabase::authentication(email, passwd, statusCode)))
    {
        // cout << status << endl;
        ur.setUserId(to_string(id));
        content = ("success\r\nid:" + ur.getUserId() + "\r\nstatus:" + to_string(statusCode) + "\r\n");
    }

    else 
    {
        // cout << status << endl;
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }


    respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_RESPOND, strlen(content.c_str()), content.c_str());
    
    return respond;
}

bool MsgParsing::logoutHandler(const MsgUnit *munit)
{
    using namespace std;
    do 
    {
        string str = getRow(munit, 0);
        if (str.size() <= 3)
            break; 
        if (IDatabase::logout(str.substr(3)))
            break;
        return true;
    } while (0);
    return false;
}

MsgUnit *MsgParsing::searchUserRespond(const MsgUnit *munit)
{
    using namespace std;

    string key = getRow(munit, 0);
    if ("" == key || key.length() <= 4)
        return nullptr;
    key = key.substr(4);
    
    int statusCode;
    pair<string, string> id_email = IDatabase::searchUser(key, statusCode);
    string content;
    if ("-1" == id_email.first)
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    else 
    {   
        content = "success\r\nid:" + id_email.first + "\r\nemail:" + id_email.second + "\r\nstatus:\r\n";
    }

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_SEARCHUSER_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::addFriendRespond(const MsgUnit *munit)
{
    using namespace std;

    vector<string> msg = getAllRows(munit);
    if (msg.size() < 2 || msg[0].length() <= 5 || msg[1].length() <= 3)
        return nullptr;

    string from = msg[0].substr(5);
    string to = msg[1].substr(3);
    string content;
    int statusCode;
    int res = IDatabase::addFriendApplication(from, to, statusCode);
    if (1 == res)
    {
        content = "success\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    else if (0 == res)
    {
        content = "conflict\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    else
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_ADDFRIEND_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::getFriendApplicationListRespond(const MsgUnit *munit)
{
    using namespace std;

    string from = getRow(munit, 0);
    if (from.size() <= 5)
        return nullptr;
    from = from.substr(5);
    int statusCode;
    bool res;
    vector<string> list = IDatabase::getFriendApplicationList(from, statusCode, res);
    string content = "";
    if (res)
    {
        content = "";
        for (const string& str : list)
        {
            content.append(str + "\r\n");
        }
    }
    else 
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_GETFRIENDAPPLICATIONLIST_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::verifyFriendApplication(const MsgUnit *munit)
{
    using namespace std;

    vector<string> msg = getAllRows(munit);
    if (msg.size() < 2)
        return nullptr;
    
    string op = msg[0];
    string id = msg[1];
    if ((op != "accept" && op != "refuse"))
        return nullptr;

    string content;
    int statusCode;
    bool res = IDatabase::friendVerification(id, op == "accept", statusCode);
    if (res)
    {
        content = "success\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    else 
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    
    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_FRIENDVERIFICATION_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::getFriendListRespond(const MsgUnit *munit)
{
    using namespace std;

    string from = getRow(munit, 0);
    if (from.size() <= 5)
        return nullptr;
    from = from.substr(5);

    bool res;
    string content;
    int statusCode;
    vector<string> resList = IDatabase::getFriendList(from, statusCode, res);
    if (res)
    {
        content = "";
        for (const string& str : resList)
        {
            content.append(str + "\r\n");
        }
    }
    else 
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_GETFRIENDLIST_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::getFolderContentRespond(const MsgUnit *munit)
{
    using namespace std;

    string target = getRow(munit, 0);
    if (target.size() <= 5)
        return nullptr;
    target = target.substr(5);

    bool res;
    int statusCode;
    string content = "";
    vector<FileInfo> v = IFileFolder::getFolderContent(target, res, statusCode);
    if (res)
    {
        for (const FileInfo& it : v)
        {
            content.append(it.fileName + "|" + to_string(it.type) + "|" + to_string(it.fileSize) + "\r\n");
        }
    }
    else 
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_GETFOLDERCONTENT_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::createFolderRespond(const MsgUnit *munit)
{
    using namespace std;

    vector<string> params = getAllRows(munit);
    if (2 != params.size() || params[0].size() <= 5 || params[1].size() <= 5)
        return nullptr;
    
    string path = params[0].substr(5);
    string name = params[1].substr(5);
    string content = "";
    int statusCode;
    if (IFileFolder::createFolder(path, name, statusCode))
    {
        bool res;
        vector<FileInfo> v = IFileFolder::getFolderContent(path, res, statusCode);
        if (res)
        {
            for (const FileInfo& it : v)
            {
                content.append(it.fileName + "|" + to_string(it.type) + "|" + to_string(it.fileSize) + "\r\n");
            }
        }
        else 
        {
            content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
        }
    }
    else 
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_CREATERFOLDER_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::renameFileFolderRespond(const MsgUnit *munit)
{
    using namespace std;

    vector<string> params = getAllRows(munit);
    if (2 != params.size() || params[0].size() <= 5 || params[1].size() <= 8)
        return nullptr;
    
    string path = params[0].substr(5);
    string newName = params[1].substr(8);
    int statusCode;
    string content = "";
    if (IFileFolder::renameFileOrFolder(path, newName, statusCode))
    {
        content = "success\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    else 
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_RENAMEFILEFOLDER_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::deleteFileFolderRespond(const MsgUnit *munit)
{
    using namespace std;

    vector<string> target = getAllRows(munit);

    int statusCode;
    string content = "";
    for (const string& it : target)
    {
        if (!IFileFolder::deleteFileOrFolder(it, statusCode))
        {
            content.append(it.substr(it.rfind('/') + 1) + "\r\n");
        }
    }
    if ("" == content)
        content = "success\r\n";

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DELETEFILEFOLDER_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::uploadFileStartRespond(const MsgUnit *munit, ConnResources& ur)
{
    using namespace std;
    using namespace filesystem;

    // 处理信息
    vector<string> fileinfo = getAllRows(munit);
    if (2 != fileinfo.size())
        return nullptr;
    
    string content;
    int statusCode;
    string path = fileinfo[1] + "/" + fileinfo[0] + ".tmp";
    int fd = IFileFolder::createFile(path, statusCode);
    if (-1 == fd)
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    else 
    {
        ur.setFd(fd);
        ur.setFilePath(path);
        content = "recv\r\n0\r\n";
    }

    MsgUnit* respond = nullptr;
    respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_START_RESPOND, strlen(content.c_str()), content.c_str());
    
    return respond;
}

MsgUnit *MsgParsing::uploadFileDataRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    string content;
    long writeBytes = write(ur.getFd(), munit->msg, munit->msgLen - 1);
    if (-1 == writeBytes)
    {
        content = "failure\r\nstatus:214\r\n";
    }
    else 
    {
        content = "recv\r\n" + to_string(writeBytes) + "\r\n";
    }

    MsgUnit* respond = nullptr;
    respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_DATA_RESPOND, strlen(content.c_str()), content.c_str());

    return respond;
}

MsgUnit *MsgParsing::uploadFileFinshRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    string content;
    int statusCode;

    string path = ur.getFilePath();
    string nameTo = path.substr(path.rfind('/'));
    nameTo = nameTo.substr(0, nameTo.rfind(".tmp"));
    if (!IFileFolder::renameFileOrFolder(path, nameTo, statusCode))
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    else 
    {
        content = "recv\r\n";
    }

    MsgUnit* respond = nullptr;
    respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_FINSH_RESPOND, strlen(content.c_str()), content.c_str());

    return respond;
}

MsgUnit *MsgParsing::uploadFileCancelRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    // 关闭资源文件
    int fd = ur.getFd();
    close(fd);

    // 删除资源文件
    int statusCode;
    string content;
    MsgUnit* respond = nullptr;
    if (IFileFolder::deleteFileOrFolder(ur.getFilePath(), statusCode))
    {
        content = "recv\r\n";
    }
    else 
    {
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    }
    respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_CANCEL_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::uploadFilePauseRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    // 关闭资源文件
    int fd = ur.getFd();
    close(fd);

    const char content[] = "recv\r\n";
    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_PAUSE_RESPOND, strlen(content), content);
    return respond;
}

MsgUnit *MsgParsing::uploadFileContinueRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    // 处理信息
    vector<string> fileinfo = getAllRows(munit);
    if (2 != fileinfo.size())
        return nullptr;
    
    string content;
    int statusCode;
    long size = -1;
    string path = fileinfo[1] + "/" + fileinfo[0] + ".tmp";
    int fd = IFileFolder::openFile(path, statusCode, &size);
    if (-1 != fd)
    {
        content = "recv\r\n" + to_string(size) + "\r\n";
        ur.setFd(fd);
        ur.setFilePath(path);
    }
    else 
        content = "failure\r\nstatus:" + to_string(statusCode) + "\r\n";
    
    MsgUnit* respond = nullptr;
    respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_CONTINUE_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::downloadFileStartRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;
    using namespace filesystem;

    // 处理信息
    vector<string> fileinfo = getAllRows(munit);
    if (1 != fileinfo.size())
        return nullptr;

    bool isSuccess = true;
    string content;
    int statusCode;
    int fd = IFileFolder::openFile(fileinfo[0], 0, statusCode);
    if (-1 == fd)
    {
        content = "status:" + to_string(statusCode) + "\r\n";
        isSuccess = false;
    }
    else 
    {
        long size = IFileFolder::getFileSize(fileinfo[0]);
        if (-1 == size)
        {
            content = "status:213\r\n";
            isSuccess = false;
        }
        else 
        {
            ur.setFd(fd);
            ur.setFilePath(fileinfo[0]);   
            content = "recv\r\n" + to_string(size) + "\r\n";
        }
    }
    
    MsgUnit* respond = nullptr;
    if (isSuccess)
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_START_RESPOND, strlen(content.c_str()), content.c_str());
    else 
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_FAILURE_RESPOND, strlen(content.c_str()), content.c_str());

    return respond;
}

MsgUnit *MsgParsing::downloadFileDataRequestRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    // 处理信息
    vector<string> fileinfo = getAllRows(munit);
    if (1 != fileinfo.size())
        return nullptr;

    char buf[1024];
    MsgUnit* respond = nullptr;
    long readBytes = read(ur.getFd(), buf, 1024);
    if (-1 == readBytes)
    {
        const char content[] = "status:215\r\n";
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_FAILURE_RESPOND, strlen(content), content);
    }
    else 
    {
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_DATA_RESPOND, readBytes, buf);
    }

    return respond;
}

MsgUnit *MsgParsing::downloadFileCancelRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    // 关闭资源文件
    int fd = ur.getFd();
    close(fd);

    // 删除资源文件
    int statusCode;
    MsgUnit* respond = nullptr;
    if (IFileFolder::deleteFileOrFolder(ur.getFilePath(), statusCode))
    {
        const char content[] = "recv\r\n";
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_CANCEL_RESPOND, strlen(content), content);
    }
    else 
    {
        string content = "status:" + to_string(statusCode) + "\r\n";
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_FAILURE_RESPOND, strlen(content.c_str()), content.c_str());
    }
    return respond;
}

// MsgUnit *MsgParsing::downloadFilePauseRespond(const MsgUnit *munit, ConnResources &ur)
// {
//     // 关闭资源文件
//     int fd = ur.getFd();
//     close(fd);

//     return nullptr;
// }

MsgUnit *MsgParsing::downloadFileContinueRespond(const MsgUnit *munit, ConnResources &ur)
{
    using namespace std;

    // 处理信息
    vector<string> fileinfo = getAllRows(munit);
    if (2 != fileinfo.size())
        return nullptr;

    MsgUnit* respond = nullptr;
    string content;
    int statusCode;
    int fd = IFileFolder::openFile(fileinfo[0], stol(fileinfo[1]), statusCode);
    if (-1 != fd)
    {
        ur.setFd(fd);
        ur.setFilePath(fileinfo[0]);
        const char content[] = "recv\r\n";
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_CONTINUE_RESPOND, strlen(content), content);
    }
    else 
    {
        string content = "status:" + to_string(statusCode) + "\r\n";
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_FAILURE_RESPOND, strlen(content.c_str()), content.c_str());
    }

    return respond;
}

bool MsgParsing::checkNumString(const std::string &str)
{
    if ("" == str)
        return false;
    if (str[0] == '0' && 1 != str.size())
        return false;
    else 
        return true;

    for (const char& c : str)
    {
        if (!isdigit(c))
            return false;
    }

    return true;
}
/*
MsgUnit *MsgParsing::parsing(const MsgUnit *munit, ConnResources& ur)
{
    // std::cout << (char*)munit->msg << std::endl;
    switch (munit->msgType)
    {
    // 登陆请求
    case MsgType::MSG_TYPE_LOGIN_REQUEST:
        // LogFunc::info("Started processing login requests from user %s.")
        return loginRespond(munit, ur);

    // 退出登陆请求
    case MsgType::MSG_TYPE_LOGOUT_REQUEST:
    {
        logoutHandler(munit);
        return nullptr;
    }

    // 查找用户请求
    case MsgType::MSG_TYPE_SEARCHUSER_REQUEST:
        return searchUserRespond(munit);

    // 添加好友请求
    case MsgType::MSG_TYPE_ADDFRIEND_REQUEST:
        return addFriendRespond(munit);

    // 获取好友申请记录请求
    case MsgType::MSG_TYPE_GETFRIENDAPPLICATIONLIST_REQUEST:
        return getFriendApplicationListRespond(munit);

    // 验证好友请求
    case MsgType::MSG_TYPE_FRIENDVERIFICATION_REQUEST:
        return verifyFriendApplication(munit);

    // 获取好友列表请求
    case MsgType::MSG_TYPE_GETFRIENDLIST_REQUEST:
        return getFriendListRespond(munit);

    // 获取文件夹内容请求
    case MsgType::MSG_TYPE_GETFOLDERCONTENT_REQUEST:
        return getFolderContentRespond(munit);

    // 创建文件夹请求
    case MsgType::MSG_TYPE_CREATERFOLDER_REQUEST:
        return createFolderRespond(munit);

    // 重命名文件/文件夹请求
    case MsgType::MSG_TYPE_RENAMEFILEFOLDER_REQUEST:
        return renameFileFolderRespond(munit);
    
    // 删除文件/文件夹请求
    case MsgType::MSG_TYPE_DELETEFILEFOLDER_REQUEST:
        return deleteFileFolderRespond(munit);

    // 文件上传任务创建请求
    case MsgType::MSG_TYPE_UPLOADFILE_START_REQUEST:
        return uploadFileStartRespond(munit, ur);

    // 文件上传数据请求
    case MsgType::MSG_TYPE_UPLOADFILE_DATA_REQUEST:
        return uploadFileDataRespond(munit, ur);

    // 文件上传上传完成请求
    case MsgType::MSG_TYPE_UPLOADFILE_FINSH_REQUEST:
        return uploadFileFinshRespond(munit, ur);

    // 取消上传请求
    case MsgType::MSG_TYPE_UPLOADFILE_CANCEL_REQUEST:
        return uploadFileCancelRespond(munit, ur);

    // 暂停上传请求
    case MsgType::MSG_TYPE_UPLOADFILE_PAUSE_REQUEST:
        return uploadFilePauseRespond(munit, ur);
    
    // 继续上传请求
    case MsgType::MSG_TYPE_UPLOADFILE_CONTINUE_REQUEST:
        return uploadFileContinueRespond(munit, ur);

    // 创建下载任务请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_START_REQUEST:
        return downloadFileStartRespond(munit, ur);
    
    // 文件下载数据请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_DATA_REQUEST:
        return downloadFileDataRequestRespond(munit, ur);

    // 取消下载请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_CANCEL_REQUEST:
        return downloadFileCancelRespond(munit, ur);
    
    // case MsgType::MSG_TYPE_DOWNLOADFILE_PAUSE_REQUEST:
    //     return nullptr;
    
    // 继续下载请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_CONTINUE_REQUEST:
        return downloadFileContinueRespond(munit, ur);

    // 未知请求
    default:
        break;
    }

    return nullptr;
}
*/

MsgUnit *MsgParsing::parsing(const MsgUnit *munit, my_bev* mbev)
{
    switch (munit->msgType)
    {
    // 登陆请求
    case MsgType::MSG_TYPE_LOGIN_REQUEST:
    {   
        LogFunc::info("Started processing login requests from user %s.", mbev->getConnectionInfo().c_str());
        return loginRespond(munit, mbev->ur);
    }

    // 退出登陆请求
    case MsgType::MSG_TYPE_LOGOUT_REQUEST:
    {
        LogFunc::info("Started processing logout requests from user %s.", mbev->getConnectionInfo().c_str());
        logoutHandler(munit);
        return nullptr;
    }

    // 查找用户请求
    case MsgType::MSG_TYPE_SEARCHUSER_REQUEST:
    {
        LogFunc::info("Started processing search user requests from user %s.", mbev->getConnectionInfo().c_str());
        return searchUserRespond(munit);
    }

    // 添加好友请求
    case MsgType::MSG_TYPE_ADDFRIEND_REQUEST:
    {
        LogFunc::info("Started processing add friend requests from user %s.", mbev->getConnectionInfo().c_str());
        return addFriendRespond(munit);
    }

    // 获取好友申请记录请求
    case MsgType::MSG_TYPE_GETFRIENDAPPLICATIONLIST_REQUEST:
    {
        LogFunc::info("Started processing get friend application list requests from user %s.", mbev->getConnectionInfo().c_str());
        return getFriendApplicationListRespond(munit);
    }

    // 验证好友请求
    case MsgType::MSG_TYPE_FRIENDVERIFICATION_REQUEST:
    {
        LogFunc::info("Started processing friend verification requests from user %s.", mbev->getConnectionInfo().c_str());
        return verifyFriendApplication(munit);
    }

    // 获取好友列表请求
    case MsgType::MSG_TYPE_GETFRIENDLIST_REQUEST:
    {
        LogFunc::info("Started processing get firend list requests from user %s.", mbev->getConnectionInfo().c_str());
        return getFriendListRespond(munit);
    }

    // 获取文件夹内容请求
    case MsgType::MSG_TYPE_GETFOLDERCONTENT_REQUEST:
    {
        LogFunc::info("Started processing get folder content requests from user %s.", mbev->getConnectionInfo().c_str());
        return getFolderContentRespond(munit);
    }

    // 创建文件夹请求
    case MsgType::MSG_TYPE_CREATERFOLDER_REQUEST:
    {
        LogFunc::info("Started processing create folder requests from user %s.", mbev->getConnectionInfo().c_str());
        return createFolderRespond(munit);
    }

    // 重命名文件/文件夹请求
    case MsgType::MSG_TYPE_RENAMEFILEFOLDER_REQUEST:
    {
        LogFunc::info("Started processing rename file or folder requests from user %s.", mbev->getConnectionInfo().c_str());
        return renameFileFolderRespond(munit);
    }
    
    // 删除文件/文件夹请求
    case MsgType::MSG_TYPE_DELETEFILEFOLDER_REQUEST:
    {
        LogFunc::info("Started processing delete file or folder requests from user %s.", mbev->getConnectionInfo().c_str());
        return deleteFileFolderRespond(munit);
    }

    // 文件上传任务创建请求
    case MsgType::MSG_TYPE_UPLOADFILE_START_REQUEST:
    {
        LogFunc::info("Started processing upload flie start requests from user %s.", mbev->getConnectionInfo().c_str());
        return uploadFileStartRespond(munit, mbev->ur);
    }

    // 文件上传数据请求
    case MsgType::MSG_TYPE_UPLOADFILE_DATA_REQUEST:
    {
        LogFunc::debug("Started processing upload file data requests from user %s.", mbev->getConnectionInfo().c_str());
        return uploadFileDataRespond(munit, mbev->ur);
    }

    // 文件上传上传完成请求
    case MsgType::MSG_TYPE_UPLOADFILE_FINSH_REQUEST:
    {
        LogFunc::info("Started processing upload file finsh requests from user %s.", mbev->getConnectionInfo().c_str());
        return uploadFileFinshRespond(munit, mbev->ur);
    }

    // 取消上传请求
    case MsgType::MSG_TYPE_UPLOADFILE_CANCEL_REQUEST:
    {
        LogFunc::info("Started processing upload file cancel requests from user %s.", mbev->getConnectionInfo().c_str());
        return uploadFileCancelRespond(munit, mbev->ur);
    }

    // 暂停上传请求
    case MsgType::MSG_TYPE_UPLOADFILE_PAUSE_REQUEST:
    {
        LogFunc::info("Started processing upload file pause requests from user %s.", mbev->getConnectionInfo().c_str());
        return uploadFilePauseRespond(munit, mbev->ur);
    }
    
    // 继续上传请求
    case MsgType::MSG_TYPE_UPLOADFILE_CONTINUE_REQUEST:
    {
        LogFunc::info("Started processing upload file continue requests from user %s.", mbev->getConnectionInfo().c_str());
        return uploadFileContinueRespond(munit, mbev->ur);
    }

    // 创建下载任务请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_START_REQUEST:
    {
        LogFunc::info("Started processing download file start requests from user %s.", mbev->getConnectionInfo().c_str());
        return downloadFileStartRespond(munit, mbev->ur);
    }
    
    // 文件下载数据请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_DATA_REQUEST:
    {
        LogFunc::debug("Started processing download file data requests from user %s.", mbev->getConnectionInfo().c_str());
        return downloadFileDataRequestRespond(munit, mbev->ur);
    }

    // 取消下载请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_CANCEL_REQUEST:
    {
        LogFunc::info("Started processing download file cancel requests from user %s.", mbev->getConnectionInfo().c_str());
        return downloadFileCancelRespond(munit, mbev->ur);
    }
    
    // case MsgType::MSG_TYPE_DOWNLOADFILE_PAUSE_REQUEST:
    //     return nullptr;
    
    // 继续下载请求
    case MsgType::MSG_TYPE_DOWNLOADFILE_CONTINUE_REQUEST:
    {
        LogFunc::info("Started processing download file continue requests from user %s.", mbev->getConnectionInfo().c_str());
        return downloadFileContinueRespond(munit, mbev->ur);
    }

    // 未知请求
    default:
        break;
    }

    return nullptr;
}

std::string MsgParsing::getRow(const MsgUnit *munit, int index)
{
    using namespace std;
    vector<string> res = split_string(std::string((char*)munit->msg), "\r\n");
    if (static_cast<size_t>(index) < res.size())
        return res[index];
    return "";
}

std::vector<std::string> MsgParsing::getAllRows(const MsgUnit *munit)
{
    return split_string(std::string((char*)munit->msg), "\r\n");
}
