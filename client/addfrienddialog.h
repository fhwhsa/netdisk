#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include "friendpage.h"

#include <QDialog>
#include <QString>
#include <QPoint>

namespace Ui {
class AddFriendDialog;
}

/**
 * @brief 添加好友对话框
 */
class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QString _userId, QString _userEmail, FriendPage *_parent);
    ~AddFriendDialog();

private:
    Ui::AddFriendDialog *ui;

    FriendPage* parent;

    QString userId;
    QString userEmail;
    QPoint centerPos; // 该窗口的中心位置

    void init();

    void iniSignalSlots();

    void getSearchUserRespond(std::shared_ptr<MsgUnit> sptr);

    void getAddFriendRespond(std::shared_ptr<MsgUnit> sptr);

private slots:
    void clickTbSearch();
    void clickTbAdd();

signals:
    void _searchUser(QString key);
    void _addFriend(QString to);

};

#endif // ADDFRIENDDIALOG_H
