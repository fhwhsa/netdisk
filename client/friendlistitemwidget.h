#ifndef FRIENDLISTITEMWIDGET_H
#define FRIENDLISTITEMWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class FListItemWidget;
}

class FriendListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendListItemWidget(QString l, QString r, QString _id, QString _email, QWidget *parent = nullptr);
    ~FriendListItemWidget();

    QString getId() const;
    QString getEmail() const;
    void setRightText(QString t) const;

private:
    Ui::FListItemWidget *ui;

    QString l, r;
    QString id, email;
};

#endif // FRIENDLISTITEMWIDGET_H
