include(../gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

HEADERS += tst_src.h

SOURCES += main.cpp


INCLUDEPATH += $$PWD/../../../src
