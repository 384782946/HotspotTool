#-------------------------------------------------
#
# Project created by QtCreator 2014-12-18T00:11:42
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HotSpotTool
TEMPLATE = app


SOURCES += main.cpp\
    propertydlg.cpp \
    QGraphicsWindow.cpp \
    qmyview.cpp

HEADERS  += \
    propertydlg.h \
    QGraphicsWindow.h \
    qmyview.h

FORMS    += \
    propertydlg.ui \
    QGraphicsWindow.ui

RESOURCES += \
    res.qrc
