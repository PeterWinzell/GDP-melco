include(../gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

HEADERS += \    
    tst_src.h
HEADERS +=     ../../../src/myclass.h
HEADERS +=     ../../../src/myclass2.h

SOURCES +=     main.cpp
SOURCES +=     ../../../src/myclass.cpp
SOURCES +=     ../../../src/myclass2.cpp

#if testing a lib
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/release/ -lGoogleTestProj
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/debug/ -lGoogleTestProj
#else:unix: LIBS += -L$$OUT_PWD/../../../src/ -lGoogleTestProj

#INCLUDEPATH += $$PWD/../../../src
#DEPENDPATH += $$PWD/../../../src
