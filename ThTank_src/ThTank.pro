#-------------------------------------------------
#
# Project created by QtCreator 2015-10-29T14:52:15
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += svg
QT       += opengl
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThTank
TEMPLATE = app

SOURCES += main.cpp \
    thtank.cpp \
    source.cpp \
    sources.cpp \
    mic.cpp \
    mics.cpp \
    simulation.cpp \
    scene.cpp \
    shape.cpp \
    utils.cpp \
    wallpoint.cpp \
    wallpoints.cpp \
    plotwindow.cpp

HEADERS  += \
    thtank.h \
    source.h \
    sources.h \
    mic.h \
    mics.h \
    simulation.h \
    scene.h \
    shape.h \
    userinterface.h \
    constants.h \
    utils.h \
    wallpoint.h \
    wallpoints.h \
    plotwindow.h \
    drawlabel.h \
    plotcanvas.h

macx{
  INCLUDEPATH += /usr/local/qwt-6.1.2/lib/qwt.framework/Headers
  LIBS += -F/usr/local/qwt-6.1.2/lib -framework qwt
}

win32{
  INCLUDEPATH +="C:/qwt-6.1.2/src"
  LIBS += C:/qwt-6.1.2/lib/libqwt.a
}


CONFIG += qwt
