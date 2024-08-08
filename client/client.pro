QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfrienddialog.cpp \
    alistitemwidget.cpp \
    bubbletips.cpp \
    flistitemwidget.cpp \
    folderpage.cpp \
    friendapplicationlist.cpp \
    friendpage.cpp \
    login.cpp \
    main.cpp \
    mainpage.cpp \
    msgUnit.cpp \
    msgtools.cpp \
    progressitemwidget.cpp \
    respondwatcher.cpp \
    transmitpage.cpp

HEADERS += \
    addfrienddialog.h \
    alistitemwidget.h \
    bubbletips.h \
    flistitemwidget.h \
    folderpage.h \
    friendapplicationlist.h \
    friendpage.h \
    login.h \
    mainpage.h \
    msgUnit.h \
    msgtools.h \
    progressitemwidget.h \
    respondwatcher.h \
    statusCode.h \
    transmitpage.h

FORMS += \
    addfrienddialog.ui \
    alistitemwidget.ui \
    flistitemwidget.ui \
    folderpage.ui \
    friendapplicationlist.ui \
    friendpage.ui \
    login.ui \
    mainpage.ui \
    progressitemwidget.ui \
    transmitpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
