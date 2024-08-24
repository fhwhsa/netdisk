#include "progressitemwidget.h"
#include "ui_progressitemwidget.h"

ProgressItemWidget::ProgressItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progressItems)
{
    ui->setupUi(this);
    iniSignalSlots();
}

ProgressItemWidget::ProgressItemWidget(int tid, QString _filename, qint64 filesize, QWidget *parent) :
    taskId(tid),
    filename(_filename),
    QWidget(parent),
    ui(new Ui::progressItems)
{
    ui->setupUi(this);
    iniSignalSlots();
    init(filename, filesize);
}

ProgressItemWidget::~ProgressItemWidget()
{
    qDebug() << "delete progress";
    emit pause();
    delete ui;
}

void ProgressItemWidget::iniSignalSlots()
{
    pauseOrCont = 0;
    pauseIcon = QIcon(":/img/res/img/pause.png");
    contIcon = QIcon(":/img/res/img/continue.png");
    connect(ui->cancelBtn, &QToolButton::clicked, this, &ProgressItemWidget::cancel);
    connect(ui->pauseOrContBtn, &QToolButton::clicked, [this](){
        if (0 == pauseOrCont)
        {
            pauseOrCont = 1;
            ui->pauseOrContBtn->setIcon(contIcon);
            emit pause();
        }
        else
        {
            pauseOrCont = 0;
            ui->pauseOrContBtn->setIcon(pauseIcon);
            emit cont();
        }
    });
}

void ProgressItemWidget::init(QString _filename, qint64 filesize)
{
    filename = _filename;
    ui->filename->setText(filename);
    totalSize = filesize;
    finshSize = 0;

    if (totalSize > 1024 * 1024 * 1024)
    {
        progressSuffix = "/" + toGB(totalSize) + "GB";
        func = toGB;
    }
    else if (totalSize > 1024 * 1024)
    {
        progressSuffix = "/" + toMB(totalSize) + "MB";
        func = toMB;
    }
    else
    {
        progressSuffix = "/" + toKB(totalSize) + "KB";
        func = toKB;
    }

    ui->progress->setText("0.00" + progressSuffix);
}

void ProgressItemWidget::updateFinshSize(qint64 val)
{
    finshSize += val;
    ui->progress->setText(func(finshSize) + progressSuffix);
    if (0 != totalSize)
        ui->progressBar->setValue(1.0 * finshSize / totalSize * 100);
}

void ProgressItemWidget::setErrorMsg(QString text)
{
    ui->errorMsg->setText(text);
}

int ProgressItemWidget::getTaskId()
{
    return taskId;
}

QString ProgressItemWidget::getFileName()
{
    return filename;
}

void ProgressItemWidget::setTaskId(int tid)
{
    taskId = tid;
}

void ProgressItemWidget::clearProgress()
{
    finshSize = 0;
    ui->progressBar->setValue(0);
    taskId = -1;
}

QString toKB(qint64 v)
{
    return QString::asprintf("%.2f", v * 1.0 / 1024);
}

QString toMB(qint64 v)
{
    return QString::asprintf("%.2f", v * 1.0 / 1024 / 1024);
}

QString toGB(qint64 v)
{
    return QString::asprintf("%.2f", v * 1.0 / 1024 / 1024 / 1024);
}
