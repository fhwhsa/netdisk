#ifndef FRIENDPAGE_H
#define FRIENDPAGE_H

#include <QWidget>

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

};

#endif // FRIENDPAGE_H
