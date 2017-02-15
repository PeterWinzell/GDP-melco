QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = W3CServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    w3cserver.cpp \
    processrequesttask.cpp \
    request-handler/requesthandler.cpp \
    request-handler/subscriptionhandler.cpp

HEADERS += \
    w3cserver.h \
    processrequesttask.h \
    request-handler/requesthandler.h \
    request-handler/subscriptionhandler.h

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    ssl.qrc
