QT += core websockets
QT -= gui
QT += xml

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = VSIAdapter

SOURCES += main.cpp \
    ../../../../VSI/vehicle_signal_interface/api/vsi_list.c \
    VSSSignalinterface/vsssignalinterfaceimpl.cpp \
    ../../W3CServer/src/OpenDSHandler/opendshandler.cpp

LIBS += /home/vagrant/VSI/vehicle_signal_interface/lib/*

INCLUDEPATH += $$PWD/../../../../VSI/vehicle_signal_interface/include
INCLUDEPATH += ../../W3CServer/src/OpenDSHandler

HEADERS += \
    ../../../../VSI/vehicle_signal_interface/include/btree.h \
    ../../../../VSI/vehicle_signal_interface/include/sharedMemory.h \
    ../../../../VSI/vehicle_signal_interface/include/sharedMemoryLocks.h \
    ../../../../VSI/vehicle_signal_interface/include/utils.h \
    ../../../../VSI/vehicle_signal_interface/include/vsi.h \
    ../../../../VSI/vehicle_signal_interface/include/vsi_core_api.h \
    ../../../../VSI/vehicle_signal_interface/include/vsi_list.h \
    VSSSignalinterface/vsssignalinterface.h \
    VSSSignalinterface/vsssignalinterfaceimpl.h \
    ../../W3CServer/src/OpenDSHandler/opendshandler.h

DISTFILES +=
