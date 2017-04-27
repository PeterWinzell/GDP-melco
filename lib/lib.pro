TEMPLATE = subdirs

SUBDIRS += QJsonWebToken Logger

DEPENDPATH += $$PWD/*
INCLUDEPATH += $$PWD/QJsonWebToken \
                $$PWD/Logger


