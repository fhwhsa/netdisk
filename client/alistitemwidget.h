#ifndef ALISTITEMWIDGET_H
#define ALISTITEMWIDGET_H

#include <QWidget>

namespace Ui {
class ItemWIdget;
}

class AListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AListItemWidget(QString left_top, QString left_bottom, QString right, QString id, QString flag, QWidget *parent = nullptr);
    ~AListItemWidget();

    QString getId();
    QString getFlag();

    void setRight(QString r);

private:
    Ui::ItemWIdget *ui;
    QString id, flag;
};

#endif // ALISTITEMWIDGET_H
