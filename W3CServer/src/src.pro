QT += core websockets
QT -= gui
QT += xml

CONFIG += c++11

TARGET = W3CServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    w3cserver.cpp \
    jsonrequestparser.cpp \
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
    request-handler/unsubnotifier.cpp \
    request-handler/statushandler.cpp \
    VSSSignalinterface/websocketbroker.cpp \
    errors/errorresponse.cpp

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
    request-handler/statushandler.h \
    VSSSignalinterface/websocketbroker.h \
    authorization/authorizationmanager.h \
    errors/errorresponse.h

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    ../ssl.qrc

INCLUDEPATH += $$PWD/../../lib/QJsonWebToken \
                $$PWD/../../lib/Logger

win32:CONFIG(release, debug|release){
    LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/release/ -lqjsonwebtoken \
             -L$$OUT_PWD/../../lib/Logger/release/ -llogger
}
win32:CONFIG(debug, debug|release){
    LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/debug/ -lqjsonwebtoken \
             -L$$OUT_PWD/../../lib/Logger/debug/ -llogger
}
unix{
    LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/ -lqjsonwebtoken \
            -L$$OUT_PWD/../../lib/Logger/ -llogger
}
