#ifndef FLISTITEMWIDGET_H
#define FLISTITEMWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class FListItemWidget;
}

class FListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FListItemWidget(QString l, QString r, QString _id, QString _email, QWidget *parent = nullptr);
    ~FListItemWidget();

    QString getId() const;
    QString getEmail() const;
    void setRightText(QString t) const;

private:
    Ui::FListItemWidget *ui;

    QString l, r;
    QString id, email;
};

#endif // FLISTITEMWIDGET_H
