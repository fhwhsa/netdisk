#include "login.h"
#include "friendpage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login l;
    l.show();
//    FriendPage f;
//    f.show();
    return a.exec();
}
