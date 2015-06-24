! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

HEADERS += persondb.h \
    locationdb.h
SOURCES += persondb.cpp \
    locationdb.cpp

TARGET = db
