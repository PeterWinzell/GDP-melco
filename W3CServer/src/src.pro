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
    vissrequest.cpp \
    request-handler/requesthandler.cpp \
    request-handler/gethandler.cpp \
    request-handler/authorizationhandler.cpp \
    jwt-utility/visstokenvalidator.cpp \
    jwt-utility/qjsonwebtoken.cpp \
    request-handler/sethandler.cpp \
    request-handler/subscribehandler.cpp \
    request-handler/unsubscribeallhandler.cpp \
    request-handler/getvsshandler.cpp \
    request-handler/unsubscribehandler.cpp \
    request-handler/processrequesttask.cpp \
    request-handler/subscriptions.cpp \
    messaging/websocketwrapper.cpp \
    request-handler/unsubnofitier.cpp

HEADERS += \
    w3cserver.h \
    request-handler/requesthandler.h \
    jwt-utility/visstokenvalidator.h \
    jsonrequestparser.h \
    vissrequest.h \
    jwt-utility/qjsonwebtoken.h \
    request-handler/gethandler.h \
    request-handler/authorizationhandler.h \
    request-handler/sethandler.h \
    request-handler/subscribehandler.h \
    request-handler/unsubscribeallhandler.h \
    request-handler/getvsshandler.h \
    request-handler/unsubscribehandler.h \
    request-handler/processrequesttask.h \
    VSSSignalinterface/vsssignalinterface.h \
    messaging/websocketwrapper.h \
    request-handler/unsubnotifier.h \
    request-handler/subscriptions.h

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    ../ssl.qrc
