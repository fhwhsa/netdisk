#ifndef FRIENDAPPLICATIONLIST_H
#define FRIENDAPPLICATIONLIST_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>

class CustomWidget : public QWidget {
public:
    CustomWidget(QString left, QString right, QWidget *parent = nullptr) : QWidget(parent) {
        QLabel *leftLabel = new QLabel(left, this);
        QLabel *rightLabel = new QLabel(right, this);
        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::VLine);
        separator->setFrameStyle(QFrame::Plain | QFrame::Sunken);

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(leftLabel);
        layout->addWidget(separator);
        layout->addWidget(rightLabel);
        layout->setContentsMargins(5, 5, 5, 5);
        layout->setSpacing(5);

        setLayout(layout);
    }
};

namespace Ui {
class FriendApplicationList;
}

class FriendApplicationList : public QDialog
{
    Q_OBJECT

public:
    explicit FriendApplicationList(QStringList databaseRecord, QWidget *parent);
    ~FriendApplicationList();

private:
    Ui::FriendApplicationList *ui;

    void init(QStringList _databaseRecord);
    void iniSignalSlots();
    void addItem(QString left, QString right);

signals:

};

#endif // FRIENDAPPLICATIONLIST_H
