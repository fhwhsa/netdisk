#ifndef FRIENDAPPLICATIONLIST_H
#define FRIENDAPPLICATIONLIST_H

#include "msgUnit.h"
#include "friendpage.h"

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <memory>

namespace Ui {
class FriendApplicationList;
}

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

    void init();
    void iniSignalSlots();
    void addItem(QString lt, QString lb, QString r, QString id, QString flag);

private slots:
    void itemDoubleClicked(QListWidgetItem *item);
    void flushList(std::shared_ptr<MsgUnit> sptr);

signals:
    void getApplicaionList();

};

#endif // FRIENDAPPLICATIONLIST_H
