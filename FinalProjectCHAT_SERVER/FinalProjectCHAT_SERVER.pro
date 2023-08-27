QT       += core gui
QT       += sql
QTPLUGIN += QSQLMYSQL
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    globalparam.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    networkconfig.cpp \
    user.cpp

HEADERS += \
    database.h \
    globalparam.h \
    mainwindow.h \
    message.h \
    networkconfig.h \
    user.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
