#ifndef FRIENDAPPLICATIONLIST_H
#define FRIENDAPPLICATIONLIST_H

#include "msgUnit.h"
#include "friendpage.h"

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QLabel>
#include <QListWidgetItem>
#include <memory>
#include <QPoint>

namespace Ui {
class FriendApplicationList;
}

/**
 * @brief 好友申请列表
 */
class FriendApplicationList : public QDialog
{
    Q_OBJECT

public:
    explicit FriendApplicationList(FriendPage* _parent);
    ~FriendApplicationList();

    void refreshManually();

private:
    Ui::FriendApplicationList *ui;

    FriendPage* parent;
    QPoint centerPos;

    void init();
    void iniSignalSlots();
    void addItem(QString lt, QString lb, QString r, QString id, QString flag);

private slots:
    void itemDoubleClicked(QListWidgetItem *item);
    void flushList(std::shared_ptr<MsgUnit> sptr);

signals:
    /**
     * @brief 信号发送后向服务器发送一个获取好友申请列表的请求
     */
    void getApplicaionList();

    /**
     * @brief 信号发送后向服务器发送一个好友验证请求
     * @param regId 要处理的好友申请记录id
     * @param flag true表示同意
     */
    void verifyFriend(QString regId, bool flag);

};

#endif // FRIENDAPPLICATIONLIST_H
