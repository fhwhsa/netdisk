#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "folderpage.h"
#include "transmitpage.h"
#include "friendpage.h"
#include "settingdialog.h"

#include <QWidget>
#include <QHash>
#include <QToolButton>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <memory>

typedef unsigned int uint;

namespace Ui {
class MainPage;
}

/**
 * @brief 主界面，同时负责与服务器进行通信
 */
class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QString _userId, QString _userEmail, QTcpSocket* _socket, QWidget *parent = nullptr);
    ~MainPage();

private:
    Ui::MainPage *ui;

    /**
     * @brief 初始化
     */
    void init();

    /**
     * @brief 初始化信号与槽
     */
    void iniSignalSlots();

    QHash<QToolButton*, QPair<QIcon, QIcon>> btnIcon;   ///< 存储按钮选中/未选中的图标地址
    QToolButton* currSelectedBtn;   ///< 记录当前选中的图标

    FolderPage* folderPage;
    TransmitPage* transmitPage;
    FriendPage* friendPage;

    QTcpSocket* socket;
    QString userId;
    QString userEmail;

    QByteArray buffer;      ///< 存储socket读到的数据
    uint waitSize;          ///< 等待字节数
    MsgUnit* revMunit;

private slots:
    /* 按钮槽函数 */
    void clickTbfolder();
    void clickTbtransmit();
    void clickTbfriend();
    void clickTblogout();
    void clickTbsetting();
    void clickTbUserInfo();

    /**
     * @brief 发送数据给服务器
     * @param munit 要发送的数据单元
     */
    void sendMsg(MsgUnit* munit);

    /**
     * @brief 接收服务器数据处理函数
     * @param munit
     */
    void recvMsg();

signals:
    void mainPageClosed();

    /**
     * @brief 由子功能页接收，并判断该数据单元是否是其所需
     * @param munit
     */
    void newMunit(std::shared_ptr<MsgUnit> munit);
};

#endif // MAINPAGE_H
