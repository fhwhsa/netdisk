#include "login.h"
#include "mainpage.h"
#include "folderpage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainPage m;
    m.show();
    return a.exec();
}
