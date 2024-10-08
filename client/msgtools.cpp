#include "msgtools.h"

#include <string>
#include <QDebug>

MsgTools::MsgTools()
{

}

MsgUnit* MsgTools::msgHandler(const QString &msg, MsgType type)
{
    std::string sstr = msg.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(sstr.c_str());
    MsgUnit* munit = MsgUnit::make_dataunit(type, num, cstr);
    return munit;
}

MsgUnit *MsgTools::generateLoginRequest(QString email, QString passwd)
{
    QString str = QString("email:%1\r\npasswd:%2\r\n").arg(email).arg(passwd);
    return msgHandler(str, MsgType::MSG_TYPE_LOGIN_REQUEST);
}

MsgUnit *MsgTools::generateLogoutRequest(QString userId)
{
    QString str = QString("id:%1\r\n").arg(userId);
    return msgHandler(str, MsgType::MSG_TYPE_LOGOUT_REQUEST);
}

MsgUnit *MsgTools::generateSearchUserRequest(QString key)
{
    QString str = QString("key:%1\r\n").arg(key);
    return msgHandler(str, MsgType::MSG_TYPE_SEARCHUSER_REQUEST);
}

MsgUnit *MsgTools::generateAddFriendRequest(QString from, QString to)
{
    QString str = QString("from:%1\r\nto:%2\r\n").arg(from).arg(to);
    return msgHandler(str, MsgType::MSG_TYPE_ADDFRIEND_REQUEST);
}

MsgUnit *MsgTools::generateGetFriendApplicationRequest(QString from)
{
    QString str = QString("from:%1\r\n").arg(from);
    return msgHandler(str, MSG_TYPE_GETFRIENDAPPLICATIONLIST_REQUEST);
}

MsgUnit *MsgTools::generateVerifyFriendRequest(QString regId, bool flag)
{
    QString str = QString("%1\r\n%2\r\n").arg(flag ? "accept" : "refuse").arg(regId);
    return msgHandler(str, MSG_TYPE_FRIENDVERIFICATION_REQUEST);
}

MsgUnit *MsgTools::generateGetFriendListRequest(QString from)
{
    QString str = QString("from:%1\r\n").arg(from);
    return msgHandler(str, MsgType::MSG_TYPE_GETFRIENDLIST_REQUEST);
}

MsgUnit *MsgTools::generateGetFolderContentRequest(QString path)
{
    QString str = QString("path:%1\r\n").arg(path);
    return msgHandler(str, MsgType::MSG_TYPE_GETFOLDERCONTENT_REQUEST);
}

MsgUnit *MsgTools::generateCreateFolderRequest(QString path, QString name)
{
    QString str = QString("path:%1\r\nname:%2\r\n").arg(path).arg(name);
    return msgHandler(str, MsgType::MSG_TYPE_CREATERFOLDER_REQUEST);
}

MsgUnit *MsgTools::generateRenameFileOrFolderRequest(QString path, QString newName)
{
    QString str = QString("path:%1\r\nnewName:%2\r\n").arg(path).arg(newName);
    return msgHandler(str, MsgType::MSG_TYPE_RENAMEFILEFOLDER_REQUEST);
}

MsgUnit *MsgTools::generateDeleteFileOrFolderRequest(QList<QString> paths)
{
    QString str;
    for (const QString& it : paths)
    {
        str.append(QString(it + "\r\n"));
    }
    return msgHandler(str, MsgType::MSG_TYPE_DELETEFILEFOLDER_REQUEST);
}

MsgUnit *MsgTools::generateUploadFileStartRequest(QString filename, QString path)
{
    QString str = QString("%1\r\n%2\r\n").arg(filename).arg(path);
    return msgHandler(str, MsgType::MSG_TYPE_UPLOADFILE_START_REQUEST);
}

MsgUnit *MsgTools::generateUploadFileDataRequest(qint64 size, const char *buffer)
{
    return MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_DATA_REQUEST, size, buffer);
}

MsgUnit *MsgTools::generateUploadFileFinshRequest()
{
    return MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_FINSH_REQUEST, 0, "/0");
}

MsgUnit *MsgTools::generateUploadFileCancelRequest()
{
    return MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_CANCEL_REQUEST, 0, "/0");
}

MsgUnit *MsgTools::generateUploadFilePauseRequest()
{
    return MsgUnit::make_dataunit(MsgType::MSG_TYPE_UPLOADFILE_PAUSE_REQUEST, 0, "/0");
}

MsgUnit *MsgTools::generateUploadFileContinueRequest(QString filename, QString path)
{
    QString str = QString("%1\r\n%2\r\n").arg(filename).arg(path);
    return msgHandler(str, MsgType::MSG_TYPE_UPLOADFILE_CONTINUE_REQUEST);
}

MsgUnit *MsgTools::generateDownloadFileStartRequest(QString filePath)
{
    QString str = filePath + "\r\n";
    return msgHandler(str, MsgType::MSG_TYPE_DOWNLOADFILE_START_REQUEST);
}

MsgUnit *MsgTools::generateDownloadFileDataRequest(qint64 hasDownloadSize)
{
    QString str = QString("%1\r\n").arg(hasDownloadSize);
    return msgHandler(str, MsgType::MSG_TYPE_DOWNLOADFILE_DATA_REQUEST);
}

MsgUnit *MsgTools::generateDownloadFileCancelRequest()
{
    return MsgUnit::make_dataunit(MsgType::MSG_TYPE_DOWNLOADFILE_CANCEL_REQUEST, 0, "/0");
}

MsgUnit *MsgTools::generateDownloadFileContinueRequest(QString downloadPath, qint64 alreadyDownloadSize)
{
    QString str = QString("%1\r\n%2\r\n").arg(downloadPath).arg(alreadyDownloadSize);
    return msgHandler(str, MsgType::MSG_TYPE_DOWNLOADFILE_CONTINUE_REQUEST);
}


QString MsgTools::getRow(const MsgUnit *munit, int index)
{
    QStringList strList = QString((char*)munit->msg).split("\r\n");
    if (index >= strList.size() - 1)
        return "";
    return strList[index];
}

QStringList MsgTools::getAllRows(const MsgUnit *munit)
{
    return QString((char*)munit->msg).split("\r\n");
}


