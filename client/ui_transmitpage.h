/********************************************************************************
** Form generated from reading UI file 'transmitpage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSMITPAGE_H
#define UI_TRANSMITPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransmitPage
{
public:
    QLabel *label;

    void setupUi(QWidget *TransmitPage)
    {
        if (TransmitPage->objectName().isEmpty())
            TransmitPage->setObjectName(QString::fromUtf8("TransmitPage"));
        TransmitPage->resize(645, 374);
        label = new QLabel(TransmitPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(280, 210, 111, 41));

        retranslateUi(TransmitPage);

        QMetaObject::connectSlotsByName(TransmitPage);
    } // setupUi

    void retranslateUi(QWidget *TransmitPage)
    {
        TransmitPage->setWindowTitle(QCoreApplication::translate("TransmitPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("TransmitPage", "\344\274\240\350\276\223\351\241\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransmitPage: public Ui_TransmitPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSMITPAGE_H
