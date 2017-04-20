QT += core websockets
QT -= gui
QT += xml

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = OpenDSAdapter

SOURCES += main.cpp \
    OpenDSHandler/opendshandler.cpp \
    signalserver.cpp



HEADERS += \
    VSSSignalinterface/vsssignalinterface.h \
    OpenDSHandler/opendshandler.h \
    signalserver.h


