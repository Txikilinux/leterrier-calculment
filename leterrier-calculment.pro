# -------------------------------------------------
# Project created by QtCreator 2010-07-13T16:03:17
# -------------------------------------------------
system(ccache -V):QMAKE_CXX = ccache g++

QT += script network xml

TARGET = leterrier-calculment
TEMPLATE = app

SOURCES += main.cpp \
    interface.cpp \
    exercice.cpp \
    baudruche.cpp \
    boutonspolygone.cpp \
    sauvegardelog.cpp \
    editeur.cpp \
    interfacecompetence.cpp \
    abuleduexercicev0.cpp \
    abuleduaproposv0.cpp \
    abuledulanceurv1.cpp \
    exercicerepechage.cpp \
    exercicemaisonnombres.cpp \
    pixmapmaison.cpp \
    exercicerepechagemaisonnombres.cpp \
    abuledulineeditv0.cpp \
    lib/abuleduexportpdfv1/abuleduexportpdfv1.cpp

HEADERS += interface.h \
    exercice.h \
    baudruche.h \
    boutonspolygone.h \
    sauvegardelog.h \
    editeur.h \
    interfacecompetence.h \
    abuleduexercicev0.h \
    abuleduaproposv0.h \
    abuledulanceurv1.h \
    exercicerepechage.h \
    exercicemaisonnombres.h \
    pixmapmaison.h \
    version.h \
    exercicerepechagemaisonnombres.h \
    abuledulineeditv0.h \
    lib/abuleduexportpdfv1/abuleduexportpdfv1.h

FORMS += interface.ui \
    exercice.ui \
    editeur.ui \
    interfacecompetence.ui \
    abuleduaproposv0.ui \
    abuledulanceurv1.ui

RESOURCES +=

win32{
  OTHER_FILES += windows/windows.rc
  RC_FILE = windows/windows.rc
}

macx {
 ICON = macos/icones/leterrier-calculment.icns
}


include(lib/abuleduapplicationv1/abuleduapplicationv1.pri)
include(lib/abuleduconstantesv1/abuleduconstantesv1.pri)
include(lib/abuledunetworkaccessmanagerv1/abuledunetworkaccessmanagerv1.pri)
include(lib/abuledumessageboxv1/abuledumessageboxv1.pri)
include(lib/abuleduidentitesv1/abuleduidentitesv1.pri)
include(lib/abuleduflatboutonv1/abuleduflatboutonv1.pri)
include(lib/maia/maia.pri)
include(lib/abuleduexercicev1/abuleduexercicev1.pri)
include(lib/abuledusplashscreenv1/abuledusplashscreenv1.pri)
include(lib/abuledufilev1/abuledufilev1.pri)
include(lib/abuledusingletonv1/abuledusingletonv1.pri)
include(lib/abuledupluginloaderv1/abuledupluginloaderv1.pri)
