QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = W3CServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    w3cserver.cpp \
    request-handler/requesthandler.cpp \
    jwt-utility/visstokenvalidator.cpp \
    jwt-utility/qjsonwebtoken.cpp \
    request-handler/processrequesttask.cpp \
    request-handler/gethandler.cpp

HEADERS += \
    w3cserver.h \
    request-handler/requesthandler.h \
    jwt-utility/visstokenvalidator.h \
    jwt-utility/qjsonwebtoken.h \
    request-handler/processrequesttask.h \
    request-handler/gethandler.h

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    ../ssl.qrc
