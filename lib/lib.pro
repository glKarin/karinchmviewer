######################################################################
# Automatically generated by qmake (2.01a) ?? 6? 1 20:58:05 2015
######################################################################

TEMPLATE = lib
CONFIG += plugin qt
TARGET = kchmfile
DEPENDPATH += . libchmfile chmlib-0.40/src
INCLUDEPATH += . libchmfile chmlib-0.40/src

MOC_DIR = ../.moc/libkchmfile
OBJECTS_DIR = ../.obj/libkchmfile

# Input
HEADERS += libchmfile/bitfiddle.h \
           libchmfile/libchmfile.h \
           libchmfile/libchmfileimpl.h \
           libchmfile/libchmsearchengine.h \
           libchmfile/libchmsearchengine_impl.h \
           libchmfile/libchmsearchengine_indexing.h \
           libchmfile/libchmtextencoding.h \
           libchmfile/libchmtocimage.h \
           libchmfile/libchmurlfactory.h \
           chmlib-0.40/src/chm_lib.h \
           chmlib-0.40/src/lzx.h
SOURCES += libchmfile/libchmfile.cpp \
           libchmfile/libchmfile_search.cpp \
           libchmfile/libchmfileimpl.cpp \
           libchmfile/libchmsearchengine.cpp \
           libchmfile/libchmsearchengine_impl.cpp \
           libchmfile/libchmsearchengine_indexing.cpp \
           libchmfile/libchmtextencoding.cpp \
           libchmfile/libchmtocimage.cpp \
           chmlib-0.40/src/chm_lib.c \
           chmlib-0.40/src/lzx.c

lib.files = libkchmfile.so
lib.path = /usr/lib

INSTALLS += lib
