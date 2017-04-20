QT += core websockets
QT -= gui
QT += xml

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = OpenDSAdapter

SOURCES += main.cpp \
    VSSSignalinterface/vsssignalinterfaceimpl.cpp \
    ../../W3CServer/src/OpenDSHandler/opendshandler.cpp \
    signalserver.cpp


INCLUDEPATH += $$PWD/../../W3CServer/src/OpenDSHandler

HEADERS += \
    VSSSignalinterface/vsssignalinterface.h \
    VSSSignalinterface/vsssignalinterfaceimpl.h \
    ../../W3CServer/src/OpenDSHandler/opendshandler.h \
    signalserver.h


