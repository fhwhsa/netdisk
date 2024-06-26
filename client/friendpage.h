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

private:
    Ui::FriendPage *ui;

    QString userId;
    QString userEmail;

    void init();
    void iniSignalSlots();

private slots:
    void clickTbNewFriend();
    void clickTbSearch();
    void clickTbClear();
    void clickTbSend();

public slots:
    void getMsg(std::shared_ptr<MsgUnit> sptr);

signals:
    /**
     * @brief
     * @param munit 要发送给服务器的数据单元
     */
    void _sendMsg(MsgUnit* munit);

    void respondSearch(std::shared_ptr<MsgUnit> sptr);
    void respondAddFriend(std::shared_ptr<MsgUnit> sptr);

};

#endif // FRIENDPAGE_H
