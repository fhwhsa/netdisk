#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QDialog>

#include "friendpage.h"

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(FriendPage *_parent = nullptr);
    ~AddFriendDialog();

private:
    Ui::AddFriendDialog *ui;

    FriendPage* parent;

    void init();

    void iniSignalSlots();

signals:
    void _searchUser(QString key);

};

#endif // ADDFRIENDDIALOG_H
