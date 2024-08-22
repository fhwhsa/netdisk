#ifndef FOLDERPAGE_H
#define FOLDERPAGE_H

#include "msgUnit.h"

#include <QWidget>
#include <QSize>
#include <QString>
#include <QListWidgetItem>
#include <QList>

namespace Ui {
class FolderPage;
}

class FolderPage : public QWidget
{
    Q_OBJECT

public:
    explicit FolderPage(QString _userId, QString _userEmail, QWidget *parent = nullptr);
    ~FolderPage();

    void refreshFileListManually();

private:
    Ui::FolderPage *ui;

    void init();
    void iniSignalSlots();

    QString currPath;
    QString userId;
    QString userEmail;
    QList<QString> backList;

    static QSize itemSize;  ///< 文件列表中单项大小，值为（0,50）    

    void flushFileList(std::shared_ptr<MsgUnit> sptr);

private slots:
    /*按钮槽函数*/
    void clickTbAddFolder();
    void clickTbDelete();
    void clickTbRename();
    void clickTbDownload();
    void clickTbUpload();
    void clickTbFlush();
    void clickTbShare();
    void clickTbBack();

    void itemDoubleClick(QListWidgetItem* item);

public slots:
    /**
     * @brief 接收服务器发来的消息，检查是否是自己需要的，如果是则进一步处理，否则忽略
     * @param sptr 接收到的消息单元
     */
    void getMsg(std::shared_ptr<MsgUnit> sptr);

signals:
    void getFolderContentRespond(std::shared_ptr<MsgUnit> sptr);

    void getCreateFolderRespond(std::shared_ptr<MsgUnit> sptr);

    void getDeleteFileOrFolderRespond(std::shared_ptr<MsgUnit> sptr);

    void getRenameFileOrFolderRespond(std::shared_ptr<MsgUnit> sptr);

    void getUploadFileRespond(std::shared_ptr<MsgUnit> sptr);

    void deliverUploadTask(QString filepath, QString diskPath);

    void deliverDownloadTask(QString filepath, qint64 filesize);

    /**
     * @brief 向服务器发送信息，有MainPage类代理
     * @param munit 要发送给服务器的数据单元
     */
    void _sendMsg(MsgUnit* munit);

};

#endif // FOLDERPAGE_H
