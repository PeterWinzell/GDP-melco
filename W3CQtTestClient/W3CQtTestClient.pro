TEMPLATE = subdirs

SUBDIRS += src
SUBDIRS += ../lib

src.depends = ../lib
