TEMPLATE = subdirs

SUBDIRS += src
SUBDIRS += ../lib

src.depends = ../lib

CONFIG(debug, debug|release) {
    SUBDIRS += tests
}
