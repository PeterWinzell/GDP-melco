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
    request-handler/sethandler.cpp \
    request-handler/subscribehandler.cpp \
    request-handler/unsubscribeallhandler.cpp \
    request-handler/getvsshandler.cpp \
    request-handler/unsubscribehandler.cpp \
    request-handler/processrequesttask.cpp \
    request-handler/subscriptions.cpp \
    messaging/websocketwrapper.cpp \
    OpenDSTCPClient/opendstcpclient.cpp \
    VSSSignalinterface/vsssignalinterfaceimpl.cpp \
    request-handler/unsubnotifier.cpp

HEADERS += \
    w3cserver.h \
    request-handler/requesthandler.h \
    jwt-utility/visstokenvalidator.h \
    jsonrequestparser.h \
    vissrequest.h \
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
    request-handler/subscriptions.h \
    OpenDSTCPClient/opendstcpclient.h \
    VSSSignalinterface/vsssignalinterfaceimpl.h

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    ../ssl.qrc

INCLUDEPATH += $$PWD/../../lib/QJsonWebToken

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/QJsonWebToken/release/ -lqjsonwebtoken
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/QJsonWebToken/debug/ -lqjsonwebtoken
else:unix:!macx: LIBS += -L$$PWD/../../lib/QJsonWebToken/ -lqjsonwebtoken
