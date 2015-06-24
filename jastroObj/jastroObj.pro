! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

INCLUDEPATH += ../swisseph

HEADERS += aspect.h \
    horoscope.h \
    constants.h \
    synastrie.h \
    composite.h \
    combine.h \
    solar.h \
    partnerprofile.h

SOURCES += aspect.cpp \
    horoscope.cpp \
    synastrie.cpp \
    composite.cpp \
    combine.cpp \
    solar.cpp \
    partnerprofile.cpp

LIBS += -L../swisseph -lswe

TARGET = jao

OTHER_FILES += \
    ../doc/combine.txt
