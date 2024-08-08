#ifndef TRANSMITPAGE_H
#define TRANSMITPAGE_H

#include "msgUnit.h"

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QString>
#include <QThread>


class UploadTask : public QThread
{
public:
    UploadTask();
    void run() override;

};


namespace Ui {
class TransmitPage;
}

class TransmitPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitPage(QWidget *parent = nullptr);
    ~TransmitPage();

    bool addUploadTask(QString filepath);

private:
    Ui::TransmitPage *ui;

    QListWidget* currListWidget;

    int maxUploadTaskNum = 2;

    void init();
    void iniSignalSlots();

public slots:
    /**
     * @brief 接收服务器发来的消息，检查是否是自己需要的，如果是则进一步处理，否则忽略
     * @param sptr 接收到的消息单元
     */
    void getMsg(std::shared_ptr<MsgUnit> sptr);
    void clickUploadListBtn();
    void clickDownloadListBtn();
    void clickFinshListBtn();

signals:
    /**
     * @brief 向服务器发送信息，有MainPage类代理
     * @param munit 要发送给服务器的数据单元
     */
    void _sendMsg(MsgUnit* munit);
};

#endif // TRANSMITPAGE_H
