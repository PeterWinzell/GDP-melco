TEMPLATE = subdirs

SUBDIRS += src
SUBDIRS += ../lib

CONFIG(debug, debug|release) {
    SUBDIRS += tests
}
