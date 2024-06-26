#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include "friendpage.h"

#include <QDialog>
#include <QString>

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QString _userId, QString _userEmail, FriendPage *_parent = nullptr);
    ~AddFriendDialog();

private:
    Ui::AddFriendDialog *ui;

    FriendPage* parent;

    QString userId;
    QString userEmail;

    void init();

    void iniSignalSlots();

private slots:
    void clickTbSearch();
    void clickTbAdd();
    void getSearchUserRespond(std::shared_ptr<MsgUnit> sptr);
    void getAddFriendRespond(std::shared_ptr<MsgUnit> sptr);

signals:
    void _searchUser(QString key);
    void _addFriend(QString to);

};

#endif // ADDFRIENDDIALOG_H
