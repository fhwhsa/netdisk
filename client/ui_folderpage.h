/********************************************************************************
** Form generated from reading UI file 'folderpage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOLDERPAGE_H
#define UI_FOLDERPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FolderPage
{
public:
    QLabel *label;

    void setupUi(QWidget *FolderPage)
    {
        if (FolderPage->objectName().isEmpty())
            FolderPage->setObjectName(QString::fromUtf8("FolderPage"));
        FolderPage->resize(400, 300);
        label = new QLabel(FolderPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 80, 75, 22));

        retranslateUi(FolderPage);

        QMetaObject::connectSlotsByName(FolderPage);
    } // setupUi

    void retranslateUi(QWidget *FolderPage)
    {
        FolderPage->setWindowTitle(QCoreApplication::translate("FolderPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("FolderPage", "\346\226\207\344\273\266\350\265\204\346\272\220\351\241\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FolderPage: public Ui_FolderPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOLDERPAGE_H
