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
    w3ctestclienthandler.cpp \
    testcasedescriptions.cpp

HEADERS += \
    w3ctestclient.h \
    getvisstestdatajson.h \
    w3ctestclienthandler.h \
    testresult.h \
    testcase.h \
    clientreport.h \
    testcasedescriptions.h

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
