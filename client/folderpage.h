#ifndef FOLDERPAGE_H
#define FOLDERPAGE_H

#include "msgUnit.h"

#include <QWidget>

namespace Ui {
class FolderPage;
}

class FolderPage : public QWidget
{
    Q_OBJECT

public:
    explicit FolderPage(QWidget *parent = nullptr);
    ~FolderPage();

private:
    Ui::FolderPage *ui;

signals:

};

#endif // FOLDERPAGE_H
