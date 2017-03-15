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

HEADERS += \
    w3ctestclient.h \
    getvisstestdatajson.h \

INCLUDEPATH += $$PWD/../../lib/QJsonWebToken

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/release/ -lqjsonwebtoken
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/debug/ -lqjsonwebtoken
else:unix: LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/ -lqjsonwebtoken
