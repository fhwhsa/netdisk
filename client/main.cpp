#include "login.h"
#include "friendpage.h"
#include "addfrienddialog.h"
#include "mainpage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login l;
    l.show();
    return a.exec();
}
