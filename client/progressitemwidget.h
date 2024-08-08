#ifndef PROGRESSITEMWIDGET_H
#define PROGRESSITEMWIDGET_H

#include <QWidget>
#include <QFile>

namespace Ui {
class progressItems;
}

class ProgressItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressItemWidget(QFile* _file, QString filename, QWidget *parent = nullptr);
    ~ProgressItemWidget();

private:
    Ui::progressItems *ui;

    QFile* file;

    qint64 totalSize;
    qint64 finshSize;

    void init();
};

#endif // PROGRESSITEMWIDGET_H
