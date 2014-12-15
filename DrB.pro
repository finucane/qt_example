#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30T10:10:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrB
TEMPLATE = app


SOURCES += main.cpp \
    BoardWindow.cpp \
    insist.cpp \
    Application.cpp \
    BoardScene.cpp \
    BoardView.cpp \
    PieceList.cpp \
    PieceGraphicsItem.cpp \
    Engine.cpp

HEADERS  += \
    BoardWindow.h \
    insist.h \
    Application.h \
    BoardScene.h \
    BoardView.h \
    PieceList.h \
    PieceGraphicsItem.h \
    Engine.h

RESOURCES += \
    resources.qrc

CONFIG += c++11
