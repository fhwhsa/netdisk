#include "progressitemwidget.h"
#include "ui_progressitemwidget.h"

ProgressItemWidget::ProgressItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progressItems)
{
    ui->setupUi(this);
    iniSignalSlots();
}

ProgressItemWidget::ProgressItemWidget(int _taskId, QString _filename, qint64 filesize, QWidget *parent) :
    filename(_filename),
    taskId(_taskId),
    QWidget(parent),
    ui(new Ui::progressItems)
{
    ui->setupUi(this);
    iniSignalSlots();
    init(taskId, filename, filesize);
}

ProgressItemWidget::~ProgressItemWidget()
{
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

void ProgressItemWidget::init(int _taskId, QString _filename, qint64 filesize)
{
    filename = _filename;
    taskId = _taskId;
    ui->filename->setText(filename);
    totalSize = filesize;
    finshSize = 0;

    if (totalSize > 1024)
    {
        ui->unit->setText("KB");
        func = toKB;
    }
    else if (totalSize > 1024 * 1024)
    {
        ui->unit->setText("MB");
        func = toMB;
    }
    else
    {
        ui->unit->setText("GB");
        func = toGB;
    }
    ui->totalSize->setText("/" + func(totalSize));
    ui->currSize->setText("0.00");
}

void ProgressItemWidget::updateFinshSize(qint64 val)
{
    finshSize += val;
    ui->currSize->setText(func(finshSize));
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
