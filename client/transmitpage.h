#ifndef TRANSMITPAGE_H
#define TRANSMITPAGE_H

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
};

#endif // TRANSMITPAGE_H
