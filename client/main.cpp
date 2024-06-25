#include "login.h"
#include "friendpage.h"
#include "addfrienddialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login l;
    l.show();
//    FriendPage f;
//    f.show();
//    AddFriendDialog afd;
//    afd.show();    
    return a.exec();
}
