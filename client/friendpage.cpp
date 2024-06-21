#include "friendpage.h"
#include "ui_friendpage.h"

FriendPage::FriendPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendPage)
{
    ui->setupUi(this);
}

FriendPage::~FriendPage()
{
    delete ui;
}
