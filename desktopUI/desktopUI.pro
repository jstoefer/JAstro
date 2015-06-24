Template = app

INCLUDEPATH += ../jastroObj
INCLUDEPATH += ../swisseph
INCLUDEPATH += ../dataBase

HEADERS +=  radixwheel.h \
            sidepanel.h \
            controller.h \
            customqdateedit.h \
            muiwin.h

SOURCES +=  radixwheel.cpp \
            sidepanel.cpp \
            controller.cpp \
            customqdateedit.cpp \
            main.cpp \
            muiwin.cpp

LIBS += -L../swisseph -L../dataBase -L../jastroObj -lswe -ldb -ljao

QT += printsupport widgets

TARGET = jastro

TRANSLATIONS = radixwheel_de.ts \
    radixwheel_en.ts

OTHER_FILES += android/AndroidManifest.xml \
               TODO.txt

ANDROID_EXTRA_LIBS += /home/jan/workspace/build-jastro-Android_Qt_5_3_0-Debug/swisseph/libswe.so
ANDROID_EXTRA_LIBS += /home/jan/workspace/build-jastro-Android_Qt_5_3_0-Debug/dataBase/libdb.so
ANDROID_EXTRA_LIBS += /home/jan/workspace/build-jastro-Android_Qt_5_3_0-Debug/jastroObj/libjao.so

RESOURCES += \
    data.qrc \
    img.qrc
