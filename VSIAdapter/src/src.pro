QT += core websockets
QT -= gui
QT += xml

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = VSIAdapter

SOURCES += main.cpp \
    VSSSignalinterface/vsssignalinterfaceimpl.cpp \
    ../../W3CServer/src/OpenDSHandler/opendshandler.cpp

LIBS += ../../lib/vsi/*.so

INCLUDEPATH += ../../lib/vsi
INCLUDEPATH += ../../W3CServer/src/OpenDSHandler

HEADERS += \
    ../../lib/vsi/vsi_list.h \
    ../../lib/vsi/vsi.h \
    ../../lib/vsi/sharedMemory.h \
    ../../lib/vsi/utils.h \
    ../../lib/vsi/btree.h \
    ../../lib/vsi/sharedMemoryLocks.h \
    ../../lib/vsi/vsi_core_api.h \
    VSSSignalinterface/vsssignalinterface.h \
    VSSSignalinterface/vsssignalinterfaceimpl.h \
    ../../W3CServer/src/OpenDSHandler/opendshandler.h


 #   ../../../../VSI/vehicle_signal_interface/include/btree.h \
 #   ../../../../VSI/vehicle_signal_interface/include/sharedMemory.h \
 #   ../../../../VSI/vehicle_signal_interface/include/sharedMemoryLocks.h \
 #   ../../../../VSI/vehicle_signal_interface/include/utils.h \
 #   ../../../../VSI/vehicle_signal_interface/include/vsi.h \
 #   ../../../../VSI/vehicle_signal_interface/include/vsi_core_api.h \
 #   ../../../../VSI/vehicle_signal_interface/include/vsi_list.h \

DISTFILES +=
