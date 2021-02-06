QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    isingsimulation.cpp \
    latticedata.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    miniature.cpp \
    pottssimulation.cpp \
    xysimulation.cpp

HEADERS += \
    isingsimulation.h \
    latticedata.h \
    mainwindow.h \
    menu.h \
    miniature.h \
    pottssimulation.h \
    xysimulation.h

FORMS += \
    mainwindow.ui \
    menu.ui

TRANSLATIONS += \
    app_mgr_pl_PL.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
