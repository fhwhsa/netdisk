#ifndef TRANSMITPAGE_H
#define TRANSMITPAGE_H

#include "msgUnit.h"
#include "progressitemwidget.h"

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QString>
#include <QHash>
#include <QPair>
#include <QFile>
#include <QThread>
#include <QTcpSocket>
#include <QHash>
#include <QEventLoop>

namespace Ui {
class TransmitPage;
}

class TransmitPage;
class UploadWorker;
class DownloadWorker;

class TransmitPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitPage(QWidget *parent = nullptr);
    ~TransmitPage();

private:
    Ui::TransmitPage *ui;

    QListWidget* currListWidget;

    QHash<int, UploadWorker*> uploadWorkerList;
    const int maxUploadTaskNum = 2;

    QHash<int, DownloadWorker*> downloadWorkerList;
    const int maxDownloadTaskNum = 2;

    void init();
    void iniSignalSlots();

public slots:
    /**
     * @brief 接收服务器发来的消息，检查是否是自己需要的，如果是则进一步处理，否则忽略
     * @param sptr 接收到的消息单元
     */
    void getMsg(std::shared_ptr<MsgUnit> sptr);
    void addUploadTask(QString filepath, QString diskPath);
    void addDownloadTask(QString filepath, qint64 filesize);

private slots:
    void clickUploadListBtn();
    void clickDownloadListBtn();
    void clickFinshListBtn();

signals:
    /**
     * @brief 向服务器发送信息，有MainPage类代理
     * @param munit 要发送给服务器的数据单元
     */
    void _sendMsg(MsgUnit* munit);

    void getUploadFileStartRespond(std::shared_ptr<MsgUnit> sptr);
};

class UploadWorker : public QThread
{
    Q_OBJECT

public:
    explicit UploadWorker(int _tid, QString _filepath, QString _diskPath);


private:
    QFile file;
    qint64 uploadSize;
    qint64 fileSize;
    QString filename, filepath, diskpath;
    int tid;
    const int blockSize = 1024;

    bool initFile();
    void upload(QTcpSocket* socket);

protected:
    void run() override;

signals:
    void updateProgress(qint64 value);
    void workFinsh(bool isSuccess, int taskId, QString errorMsg = ""); // 工作线程结束时发起

};

class DownloadWorker : public QThread
{
    Q_OBJECT

public:
    explicit DownloadWorker(int _tid, QString _downloadPath, qint64 _fileSize, QString _storePath);

private:
    QFile file;
    qint64 downloadSize, fileSize;
    QString downloadPath, storePath, filename;
    int tid;

    void download(QTcpSocket* socket);


protected:
    void run() override;

signals:
    void updateProgress(qint64 value);
    void workFinsh(bool isSuccess, int taskId, QString errorMsg = "");

};

#endif
