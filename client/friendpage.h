#ifndef FRIENDPAGE_H
#define FRIENDPAGE_H

#include "msgUnit.h"

#include <QWidget>
#include <memory>

namespace Ui {
class FriendPage;
}

class FriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendPage(QWidget *parent = nullptr);
    ~FriendPage();

private:
    Ui::FriendPage *ui;
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

};

#endif // FRIENDPAGE_H
