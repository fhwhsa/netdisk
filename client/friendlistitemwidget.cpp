#include "friendlistitemwidget.h"
#include "ui_flistitemwidget.h"

FriendListItemWidget::FriendListItemWidget(QString l, QString r, QString _id, QString _email, QWidget *parent) :
    id(_id),
    email(_email),
    QWidget(parent),
    ui(new Ui::FListItemWidget)
{
    ui->setupUi(this);

    if (l.size() <= 15)
        ui->leftLabel->setText(l);
    else
        ui->leftLabel->setText(l.first(15) + "...");
    ui->rightLabel->setText(r);
}

FriendListItemWidget::~FriendListItemWidget()
{
    delete ui;
}

QString FriendListItemWidget::getId() const
{
    return id;
}

QString FriendListItemWidget::getEmail() const
{
    return email;
}

void FriendListItemWidget::setRightText(QString t) const
{
    ui->rightLabel->setText(t);
}
