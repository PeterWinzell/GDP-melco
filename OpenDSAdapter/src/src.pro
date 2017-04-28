QT += core websockets
QT -= gui
QT += xml

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = OpenDSAdapter

SOURCES += main.cpp \
            signalserver.cpp \
            OpenDSHandler/opendshandler.cpp

HEADERS += signalserver.h \
            OpenDSHandler/opendshandler.h

RESOURCES += \
    ../settings.qrc

