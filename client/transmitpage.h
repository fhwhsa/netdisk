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
enum WorkType
{
    NEW_WORK,
    CONTINUE_WORK
};

/**
 * @brief 传输页面
 */
class TransmitPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitPage(QWidget *parent = nullptr);
    ~TransmitPage();

private:
    Ui::TransmitPage *ui;

    QListWidget* currListWidget; ///< 维护当前展示的QListWidget对象

    QHash<int, UploadWorker*> uploadWorkerList; ///< 存储正在进行的上传任务对象
    const int maxUploadTaskNum = 2; ///< 最大同时进行的上传任务数
    /**
     * @brief uploadWorkFinshHandler
     * @param status 任务完成状态
     * @param taskId 任务id
     * @param errorMsg 错误信息，任务正常完成则为空字符串
     */
    void uploadWorkFinshHandler(int status, int taskId, QString errorMsg);

    QHash<int, DownloadWorker*> downloadWorkerList;
    const int maxDownloadTaskNum = 2;
    void downloadWorkFinshHandler(int status, int taskId, QString msg);

    void init();
    void iniSignalSlots();

public slots:
    /**
     * @brief 接收服务器发来的消息，检查是否是自己需要的，如果是则进一步处理，否则忽略
     * @param sptr 接收到的消息单元
     */
    void getMsg(std::shared_ptr<MsgUnit> sptr);

    /**
     * @brief 添加上传任务
     * @param filepath 上传文件路径
     * @param diskPath 网盘存储路径
     * @param wt 任务启动状态（新任务/继续）
     */
    void addUploadTask(QString filepath, QString diskPath, WorkType wt = WorkType::NEW_WORK);

    /**
     * @brief 添加下载任务
     * @param filepath 下载文件的网盘路径
     * @param filesize 文件大小（字节）
     * @param wt 任务启动状态（新任务/继续）
     */
    void addDownloadTask(QString filepath, qint64 filesize, WorkType wt = WorkType::NEW_WORK);

private slots:
    /* 按钮槽函数 */
    void clickUploadListBtn();
    void clickDownloadListBtn();
    void clickFinshListBtn();

signals:
    /**
     * @brief 向服务器发送信息，有MainPage类代理
     * @param munit 要发送给服务器的数据单元
     */
    void _sendMsg(MsgUnit* munit);

//    void getUploadFileStartRespond(std::shared_ptr<MsgUnit> sptr);
};

class UploadWorker : public QThread
{
    Q_OBJECT

public:
    explicit UploadWorker(int _tid, QString _filepath, QString _diskPath, WorkType _wt);
    ~UploadWorker();

private:
    QFile file; ///< 打开的上传文件
    qint64 uploadSize; ///< 已上传的大小（byte）
    qint64 fileSize; ///< 文件总大小（byte）
    QString filename, filepath, diskpath;
    int tid; ///< 任务id
    const int blockSize = 1024;
    bool isCancel; ///< 是否取消任务
    bool isPause; ///< 是否暂停任务
    WorkType wt;

    /**
     * @brief 初始化文件
     * @return
     */
    bool initFile();

    /**
     * @brief socket可读时调用
     * @param socket
     */
    void upload(QTcpSocket* socket);

protected:
    void run() override;

signals:
    void cancel();
    void pause();

    /**
     * @brief 更新上传进度
     * @param value 变化值（byte）
     */
    void updateProgress(qint64 value);

    /**
     * @brief workFinsh 工作线程结束时发出
     * @param status -1代表出错，0代表上传完成，1代表暂停，2代表取消
     * @param taskId 任务id
     * @param errorMsg 携带错误信息
     */
    void workFinsh(int status, int taskId, QString errorMsg = "");
};

class DownloadWorker : public QThread
{
    Q_OBJECT

public:
    explicit DownloadWorker(int _tid, QString _downloadPath, qint64 _fileSize, QString _storePath, WorkType _wt = WorkType::NEW_WORK);

private:
    QFile file;
    qint64 downloadSize, fileSize;
    QString downloadPath, storePath, filename;
    int tid;
    bool isCancel;
    bool isPause;
    WorkType wt;

    void download(QTcpSocket* socket);


protected:
    void run() override;

signals:
    void cancel();
    void pause();
    void updateProgress(qint64 value);

    /**
     * @brief workFinsh 工作线程结束时发出
     * @param status -1代表出错，0代表下载完成，1代表暂停，2代表取消
     * @param taskId 任务id
     * @param msg 携带信息
     */
    void workFinsh(int status, int taskId, QString msg = "");

};

#endif
