#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>

namespace Ui {
class ItemWIdget;
}

class ItemWIdget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWIdget(QString left_top, QString left_bottom, QString right, QString id, QString flag, QWidget *parent = nullptr);
    ~ItemWIdget();

    QString getId();
    QString getFlag();

private:
    Ui::ItemWIdget *ui;
    QString id, flag;
};

#endif // ITEMWIDGET_H
