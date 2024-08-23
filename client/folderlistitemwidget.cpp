#include "folderlistitemwidget.h"
#include "ui_folderlistitemwidget.h"

#include <QPixmap>
#include <QDebug>

FolderListItemWidget::FolderListItemWidget(int _type, QWidget *parent) :
    type(_type),
    QWidget(parent),
    ui(new Ui::FolderListItemWidget)
{
    ui->setupUi(this);
}

FolderListItemWidget::~FolderListItemWidget()
{
    delete ui;
}

int FolderListItemWidget::getType()
{
    return type;
}

void FolderListItemWidget::init(QString name, QString path, qint64 size)
{
    setName(name);
    setIcon(path);
    setSize(size);
}

void FolderListItemWidget::setName(QString name)
{
    ui->name->setText(name);
    fileName = name;
}

void FolderListItemWidget::setIcon(QString path)
{
    QPixmap pixmap(path);
    pixmap = pixmap.scaled(30, 30);
    ui->icon->setPixmap(pixmap);
}

void FolderListItemWidget::setSize(qint64 size)
{
    fileSize = size;
    if (-1 == size)
        ui->size->setText("");
    else
        ui->size->setText(QString("%1").arg(size));
}

QString FolderListItemWidget::getFileName()
{
    return fileName;
}

qint64 FolderListItemWidget::getFileSize()
{
    return fileSize;
}
