# -------------------------------------------------
# Project created by QtCreator 2011-03-05T19:08:08
# -------------------------------------------------
TARGET = abuledu-accueil
TEMPLATE = app
SOURCES += main.cpp \
    abuleduaccueil.cpp
HEADERS += abuleduaccueil.h
FORMS += abuleduaccueil.ui
RESOURCES += ressources.qrc

CONFIG   += static

win32 {
 QMAKE_LFLAGS += -static-libgcc
}

RC_FILE = icon.rc
