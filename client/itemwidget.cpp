#include "itemwidget.h"
#include "ui_itemwidget.h"

ItemWIdget::ItemWIdget(QString left_top, QString left_bottom, QString right, QString id, QString flag, QWidget *parent) :
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

ItemWIdget::~ItemWIdget()
{
    delete ui;
}

QString ItemWIdget::getId()
{
    return id;
}

QString ItemWIdget::getFlag()
{
    return flag;
}
