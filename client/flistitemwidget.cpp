#include "flistitemwidget.h"
#include "ui_flistitemwidget.h"

FListItemWidget::FListItemWidget(QString l, QString r, QString _id, QString _email, QWidget *parent) :
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

FListItemWidget::~FListItemWidget()
{
    delete ui;
}

QString FListItemWidget::getId() const
{
    return id;
}

QString FListItemWidget::getEmail() const
{
    return email;
}

void FListItemWidget::setRightText(QString t) const
{
    ui->rightLabel->setText(t);
}
