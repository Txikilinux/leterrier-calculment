# -------------------------------------------------
# Project created by QtCreator 2010-07-13T16:03:17
# -------------------------------------------------
system(ccache -V):QMAKE_CXX = ccache g++

QT += script network xml gui

TARGET = leterrier-calculment
TEMPLATE = app

SOURCES += main.cpp \
    interface.cpp \
    exercice.cpp \
    baudruche.cpp \
    boutonspolygone.cpp \
    sauvegardelog.cpp \
    editeur.cpp \
    abuleduexercicev0.cpp \
    abuledulanceurv1.cpp \
    exercicerepechage.cpp \
    exercicemaisonnombres.cpp \
    pixmapmaison.cpp \
    exercicerepechagemaisonnombres.cpp \
    abuledulineeditv0.cpp \
    futurelib/abuleduexportpdfv1/abuleduexportpdfv1.cpp \
    calculmentgraphicsview.cpp \
    abstractexercise.cpp \
    exerciceoperation.cpp \
    abuleduwidgetv1.cpp

HEADERS += interface.h \
    exercice.h \
    baudruche.h \
    boutonspolygone.h \
    sauvegardelog.h \
    editeur.h \
    abuleduexercicev0.h \
    abuledulanceurv1.h \
    exercicerepechage.h \
    exercicemaisonnombres.h \
    pixmapmaison.h \
    version.h \
    exercicerepechagemaisonnombres.h \
    abuledulineeditv0.h \
    futurelib/abuleduexportpdfv1/abuleduexportpdfv1.h \
    calculmentgraphicsview.h \
    leterrierstringtransition.h \
    abstractexercise.h \
    exerciceoperation.h \
    abuleduwidgetv1.h

FORMS += interface.ui \
    exercice.ui \
    editeur.ui \
    abuledulanceurv1.ui

RESOURCES += \
    calculment.qrc

win32{
  OTHER_FILES += ../windows/windows.rc
  RC_FILE = ../windows/windows.rc
}

macx {
 ICON = ../macos/icones/leterrier-calculment.icns
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
include(lib/abuledulangv1/abuledulangv1.pri)
include(lib/abuleduloggerv1/abuleduloggerv1.pri)
include(lib/abuleduloadinganimationv1/abuleduloadinganimationv1.pri)
include(lib/abuleduaproposv1/abuleduaproposv1.pri)
include(lib/abuledustackedwidgetv1/abuledustackedwidgetv1.pri)
include(lib/abuledumultimediav1/abuledumultimediav1.pri)
include(lib/abuledupicottsv1/abuledupicottsv1.pri)
include(lib/abuleduaudiov1/abuleduaudiov1.pri)
include(lib/abuledupageaccueilv1/abuledupageaccueilv1.pri)
include(lib/abuledugraphicmenuv1/abuledugraphicmenuv1.pri)
include(lib/abuledulabelv1/abuledulabelv1.pri)
include(lib/abuleducommonstatesv1/abuleducommonstatesv1.pri)
include(lib/abuledustatemachinev1/abuledustatemachinev1.pri)
include(lib/abuledutetev1/abuledutetev1.pri)
