QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = W3CServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    w3cserver.cpp \
    jsonrequestparser.cpp \
    vissrequest.cpp

HEADERS += \
    w3cserver.h \
    jsonrequestparser.h \
    vissrequest.h

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    ../ssl.qrc
