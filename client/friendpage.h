#ifndef FRIENDPAGE_H
#define FRIENDPAGE_H

#include "msgUnit.h"

#include <QWidget>
#include <memory>
#include <QString>

namespace Ui {
class FriendPage;
}

class FriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendPage(QString _userId, QString _userEmail, QWidget *parent = nullptr);
    ~FriendPage();

    void refreshFriendListManually();

private:
    Ui::FriendPage *ui;

    QString userId;
    QString userEmail;

    void init();
    void iniSignalSlots();

    /**
     * @brief 刷新好友列表
     * @param sptr
     */
    void flushFriendList(std::shared_ptr<MsgUnit> sptr);

private slots:
    /*按钮槽函数*/
    void clickTbNewFriend();
    void clickTbSearch();
    void clickTbClear();
    void clickTbSend();
    void clickTbNotification();
    void clickTbFlushFriendList();

public slots:
    /**
     * @brief 接收服务器发来的消息，检查是否是自己需要的，如果是则进一步处理，否则忽略
     * @param sptr 接收到的消息单元
     */
    void getMsg(std::shared_ptr<MsgUnit> sptr);

signals:
    /**
     * @brief 向服务器发送信息，有MainPage类代理
     * @param munit 要发送给服务器的数据单元
     */
    void _sendMsg(MsgUnit* munit);


    /*自己接收********************************************************************************************/
    void respondGetFriendList(std::shared_ptr<MsgUnit> sptr);


    /*friendpage从属类接收**********************************************************************************/

    void respondGetFriendApplication(std::shared_ptr<MsgUnit> sptr);

    /**
     * @brief 查找用户的响应，通过信号对消息分发，需要此消息的类通过接收该信号得到消息内容
     * @param sptr
     */
    void respondSearch(std::shared_ptr<MsgUnit> sptr);

    /**
     * @brief 添加好友的响应，通过信号对消息分发
     * @param sptr
     */
    void respondAddFriend(std::shared_ptr<MsgUnit> sptr);

    /**
     * @brief 同意/拒绝好友申请的响应，通过信号对消息分发
     * @param sptr
     */
    void respondVerify(std::shared_ptr<MsgUnit> sptr);

};

#endif // FRIENDPAGE_H
