#ifndef PROGRESSITEMWIDGET_H
#define PROGRESSITEMWIDGET_H

#include <QWidget>
#include <QFile>
#include <QString>
#include <QIcon>

namespace Ui {
class progressItems;
}

class ProgressItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressItemWidget(QWidget *parent = nullptr);
    explicit ProgressItemWidget(int _taskId, QString _filename, qint64 filesize, QWidget *parent = nullptr);

    void init(int _taskId, QString _filename, qint64 filesize);

    void updateFinshSize(qint64 val);

    void setErrorMsg(QString text);

    int getTaskId();

    QString getFileName();

    ~ProgressItemWidget();

private:
    Ui::progressItems *ui;

    const qint64 unit = 4096;
    qint64 totalSize;
    qint64 finshSize;
    std::function<QString(qint64)> func;

    int taskId;
    QString filename;

    int pauseOrCont; // 标识按钮状态
    QIcon pauseIcon, contIcon;

    void iniSignalSlots();

signals:
    void cancel();
    void pause();
    void cont();
};


QString toKB(qint64 v);
QString toMB(qint64 v);
QString toGB(qint64 v);

#endif // PROGRESSITEMWIDGET_H
