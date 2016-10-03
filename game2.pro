#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T20:46:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cross-square
TEMPLATE = app


SOURCES += main.cpp \
    cell.cpp \
    game.cpp

HEADERS  += \
    cell.h \
    game.h

QMAKE_CXXFLAGS += -std=c++11
