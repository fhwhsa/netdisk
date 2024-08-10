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

class UploadWorker;
class DownloadWorker;
class TransmitPage;

class TransmitPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitPage(QWidget *parent = nullptr);
    ~TransmitPage();

private:
    Ui::TransmitPage *ui;

    QListWidget* currListWidget;
    QHash<int, UploadWorker*> uploadTaskList;

    const int maxUploadTaskNum = 2;

    void init();
    void iniSignalSlots();

public slots:
    /**
     * @brief 接收服务器发来的消息，检查是否是自己需要的，如果是则进一步处理，否则忽略
     * @param sptr 接收到的消息单元
     */
    void getMsg(std::shared_ptr<MsgUnit> sptr);
    void addUploadTask(QString filepath, QString diskPath);

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
    UploadWorker(int _taskId, QString filepath, QString diskPath, ProgressItemWidget* _piw);
    ~UploadWorker();

private:
    QTcpSocket* socket;
    QFile* file;
    ProgressItemWidget* piw;
    int taskId;

    void releaseSources();

private slots:
    void recvHandler();
    void cancelTask();
    void pauseTask();
    void contTask();

protected:
    void run() override;

signals:
    void exitEventLoop();   // 退出时间循环信号
    void taskFinsh(int taskId);   // 上传任务完成信号器
};

#endif // TRANSMITPAGE_H
