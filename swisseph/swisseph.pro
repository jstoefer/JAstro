! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

HEADERS += swedate.h \
           swedll.h \
           swehouse.h \
           swejpl.h \
           sweodef.h \
           swepcalc.h \
           sweph.h \
           swephexp.h \
           swephlib.h
SOURCES += swecl.c \
           swedate.c \
           swehouse.c \
           swejpl.c \
           swemmoon.c \
           swemplan.c \
           swepcalc.c \
           swepdate.c \
           sweph.c \
           swephlib.c \
           swemptab.c
		   
CONFIG -= qt
TARGET = swe

RESOURCES += \
    ephem.qrc
