include(../gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

QT += core websockets
QT -= gui
QT += xml

HEADERS += \
    tst_jsonrequestparser.h \
    tst_w3cserver.h \
    tst_vsssignalinterface.h

SOURCES += main.cpp \
    ../../../src/w3cserver.cpp \
    ../../../src/jsonrequestparser.cpp \
    ../../../src/vissrequest.cpp \
    ../../../src/request-handler/requesthandler.cpp \
    ../../../src/request-handler/gethandler.cpp \
    ../../../src/request-handler/authorizationhandler.cpp \
    ../../../src/jwt-utility/visstokenvalidator.cpp \
    ../../../src/request-handler/sethandler.cpp \
    ../../../src/request-handler/subscribehandler.cpp \
    ../../../src/request-handler/unsubscribeallhandler.cpp \
    ../../../src/request-handler/getvsshandler.cpp \
    ../../../src/request-handler/unsubscribehandler.cpp \
    ../../../src/request-handler/processrequesttask.cpp \
    ../../../src/request-handler/subscriptions.cpp \
    ../../../src/request-handler/unsubnotifier.cpp \
    ../../../src/request-handler/statushandler.cpp \
    ../../../src/messaging/websocketwrapper.cpp \
    ../../../src/VSSSignalinterface/vsssignalinterfaceimpl.cpp \
    ../../../src/OpenDSHandler/opendshandler.cpp \
    ../../../src/VSSSignalinterface/vsiimpl.cpp


HEADERS += \
    ../../../src/w3cserver.h \
    ../../../src/request-handler/requesthandler.h \
    ../../../src/jwt-utility/visstokenvalidator.h \
    ../../../src/jsonrequestparser.h \
    ../../../src/vissrequest.h \
    ../../../src/request-handler/gethandler.h \
    ../../../src/request-handler/authorizationhandler.h \
    ../../../src/request-handler/sethandler.h \
    ../../../src/request-handler/subscribehandler.h \
    ../../../src/request-handler/unsubscribeallhandler.h \
    ../../../src/request-handler/getvsshandler.h \
    ../../../src/request-handler/unsubscribehandler.h \
    ../../../src/request-handler/processrequesttask.h \
    ../../../src/request-handler/subscriptions.h \
    ../../../src/request-handler/unsubnotifier.h \
    ../../../src/request-handler/statushandler.h \
    ../../../src/messaging/websocketwrapper.h \
    ../../../src/VSSSignalinterface/vsssignalinterface.h \
    ../../../src/VSSSignalinterface/vsssignalinterfaceimpl.h \
    ../../../src/OpenDSHandler/opendshandler.h\
    ../../../src/VSSSignalinterface/vsiimpl.h \
    ../../../../lib/vsi/vsi_list.h \
    ../../../../lib/vsi/vsi.h \
    ../../../../lib/vsi/sharedMemory.h \
    ../../../../lib/vsi/utils.h \
    ../../../../lib/vsi/btree.h \
    ../../../../lib/vsi/sharedMemoryLocks.h \
    ../../../../lib/vsi/vsi_core_api.h
INCLUDEPATH += $$PWD/../../../src/messaging
INCLUDEPATH += $$PWD/../../../src/VSSSignalinterface
INCLUDEPATH += $$PWD/../../../src/OpenDSTCPClient
INCLUDEPATH += $$PWD/../../../src/request-handler
INCLUDEPATH += $$PWD/../../../src/jwt-utility
INCLUDEPATH += $$PWD/../../../src

#if testing a lib
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/release/ -lGoogleTestProj
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/debug/ -lGoogleTestProj
#else:unix: LIBS += -L$$OUT_PWD/../../../src/ -lGoogleTestProj

INCLUDEPATH += ../../../../lib/QJsonWebToken
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../lib/QJsonWebToken/release/ -lqjsonwebtoken
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../lib/QJsonWebToken/debug/ -lqjsonwebtoken
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../../lib/QJsonWebToken/ -lqjsonwebtoken

DISTFILES += \
    data/vss_rel_1.json

LIBS += /home/vagrant/qt-projects/vsi_test/lib/vsi/*
unix:QMAKE_POST_LINK += $$quote($$QMAKE_COPY $${PWD}/data/* $${OUT_PWD})
else:win32:QMAKE_POST_LINK += $$QMAKE_COPY \"$${PWD}/data\\*\" \"$${OUT_PWD}\"
