#include "alistitemwidget.h"
#include "ui_itemwidget.h"

AListItemWidget::AListItemWidget(QString left_top, QString left_bottom, QString right, QString id, QString flag, QWidget *parent) :
    id(id),
    flag(flag),
    QWidget(parent),
    ui(new Ui::ItemWIdget)
{
    ui->setupUi(this);
    ui->left_top->setText(left_top);
    ui->left_bottom->setText(left_bottom);
    ui->right->setText(right);
}

AListItemWidget::~AListItemWidget()
{
    delete ui;
}

QString AListItemWidget::getId()
{
    return id;
}

QString AListItemWidget::getFlag()
{
    return flag;
}

void AListItemWidget::setRight(QString r)
{
    ui->right->setText(r);
}
