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
    explicit ProgressItemWidget(int tid, QString _filename, qint64 filesize, QWidget *parent = nullptr);

    void updateFinshSize(qint64 val);
    void setErrorMsg(QString text);
    void clearProgress();

    int getTaskId();    
    QString getFileName();

    void setTaskId(int tid);

    ~ProgressItemWidget();

private:
    Ui::progressItems *ui;

    qint64 totalSize;
    qint64 finshSize;
    std::function<QString(qint64)> func;

    int taskId;
    QString filename;

    QString progressSuffix;

    int pauseOrCont; // 标识按钮状态
    QIcon pauseIcon, contIcon;

    void iniSignalSlots();
    void init(QString _filename, qint64 filesize);


signals:
    void cancel();
    void pause();
    void cont();
};


QString toKB(qint64 v);
QString toMB(qint64 v);
QString toGB(qint64 v);

#endif // PROGRESSITEMWIDGET_H
