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
    signalserver.cpp \
    OpenDS/opendshandler.cpp

HEADERS += \
    VSSSignalinterface/vsssignalinterface.h \
    VSSSignalinterface/vsssignalinterfaceimpl.h \
    signalserver.h \
    OpenDS/opendshandler.h


