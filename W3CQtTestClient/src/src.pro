<<<<<<< HEAD
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
    w3ctestclienthandler.cpp

HEADERS += \
    w3ctestclient.h \
    getvisstestdatajson.h \
    w3ctestclienthandler.h \
    testresult.h \
    testcase.h

INCLUDEPATH += $$PWD/../../lib/QJsonWebToken

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/release/ -lqjsonwebtoken
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/debug/ -lqjsonwebtoken
else:unix: LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/ -lqjsonwebtoken
=======
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
    w3ctestclienthandler.cpp

HEADERS += \
    w3ctestclient.h \
    getvisstestdatajson.h \
    w3ctestclienthandler.h \
    testresult.h \
    testcase.h

INCLUDEPATH += $$PWD/../../lib/QJsonWebToken

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/release/ -lqjsonwebtoken
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/debug/ -lqjsonwebtoken
else:unix: LIBS += -L$$OUT_PWD/../../lib/QJsonWebToken/ -lqjsonwebtoken
>>>>>>> 8426b50bc0330aee1954172cdf6a7e967943eb39
