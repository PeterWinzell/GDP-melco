include(../gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

QT += core websockets
QT -= gui

HEADERS += tst_src.h \
    tst_jsonrequestparser.h \
    tst_w3cserver.h

SOURCES += main.cpp \
    ../../../src/w3cserver.cpp \
    ../../../src/jsonrequestparser.cpp \
    ../../../src/vissrequest.cpp \
    ../../../src/request-handler/requesthandler.cpp \
    ../../../src/request-handler/gethandler.cpp \
    ../../../src/request-handler/authorizationhandler.cpp \
    ../../../src/jwt-utility/visstokenvalidator.cpp \
    ../../../src/jwt-utility/qjsonwebtoken.cpp \
    ../../../src/request-handler/sethandler.cpp \
    ../../../src/request-handler/subscribehandler.cpp \
    ../../../src/request-handler/unsubscribeallhandler.cpp \
    ../../../src/request-handler/getvsshandler.cpp \
    ../../../src/request-handler/unsubscribehandler.cpp \
    ../../../src/request-handler/processrequesttask.cpp

HEADERS += \
    ../../../src/w3cserver.h \
    ../../../src/request-handler/requesthandler.h \
    ../../../src/jwt-utility/visstokenvalidator.h \
    ../../../src/jsonrequestparser.h \
    ../../../src/vissrequest.h \
    ../../../src/jwt-utility/qjsonwebtoken.h \
    ../../../src/request-handler/gethandler.h \
    ../../../src/request-handler/authorizationhandler.h \
    ../../../src/request-handler/sethandler.h \
    ../../../src/request-handler/subscribehandler.h \
    ../../../src/request-handler/unsubscribeallhandler.h \
    ../../../src/request-handler/getvsshandler.h \
    ../../../src/request-handler/unsubscribehandler.h \
    ../../../src/request-handler/processrequesttask.h

INCLUDEPATH += $$PWD/../../../src/request-handler
INCLUDEPATH += $$PWD/../../../src/jwt-utility
INCLUDEPATH += $$PWD/../../../src

#if testing a lib
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/release/ -lGoogleTestProj
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/debug/ -lGoogleTestProj
#else:unix: LIBS += -L$$OUT_PWD/../../../src/ -lGoogleTestProj

