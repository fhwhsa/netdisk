#ifndef TRANSMITPAGE_H
#define TRANSMITPAGE_H

#include "msgUnit.h"

#include <QWidget>

namespace Ui {
class TransmitPage;
}

class TransmitPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitPage(QWidget *parent = nullptr);
    ~TransmitPage();

private:
    Ui::TransmitPage *ui;

signals:

};

#endif // TRANSMITPAGE_H
