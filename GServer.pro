#-------------------------------------------------
#
# Project created by QtCreator 2014-03-18T11:24:48
#
#-------------------------------------------------

QT       += core network

QT       -= gui

QMAKE_CXXFLAGS += -std=gnu++11

TARGET = GServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mmaininstance.cpp \
    msocket.cpp

HEADERS += \
    mmaininstance.h \
    msocket.h \
    tcpcommands.h
