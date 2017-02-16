QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = W3CQtTestClient
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    w3ctestclient.cpp \
    getvisstestdatajson.cpp \
    qjsonwebtoken.cpp

HEADERS += \
    w3ctestclient.h \
    getvisstestdatajson.h \
    qjsonwebtoken.h
