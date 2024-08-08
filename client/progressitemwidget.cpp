#include "progressitemwidget.h"
#include "ui_progressitemwidget.h".h"

ProgressItemWidget::ProgressItemWidget(QFile* _file, QString filename, QWidget *parent) :
    QWidget(parent),
    file(_file),
    ui(new Ui::progressItems)
{
    ui->setupUi(this);

    ui->filename->setText(filename);
    totalSize = file->size();
    finshSize = 0;
}

ProgressItemWidget::~ProgressItemWidget()
{
    delete ui;

    if (nullptr != file)
    {
        file->close();
        delete file;
        file = nullptr;
    }
}

void ProgressItemWidget::init()
{
}
