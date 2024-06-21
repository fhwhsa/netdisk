#include "transmitpage.h"
#include "ui_transmitpage.h"

TransmitPage::TransmitPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransmitPage)
{
    ui->setupUi(this);
}

TransmitPage::~TransmitPage()
{
    delete ui;
}
