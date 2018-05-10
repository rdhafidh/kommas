#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T20:04:00
#
#-------------------------------------------------

CONFIG +=  release silent
QT       += core gui sql concurrent
QMAKE_TARGET_COMPANY += ""
QMAKE_TARGET_DESCRIPTION += ""
QMAKE_TARGET_COPYRIGHT += "Copyright (C) 2014 "
QMAKE_TARGET_PRODUCT += "Kommas .Inc"
RC_FILE =kommas.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
BREAKPAD = D:\masteraplikasi\transferh11nov\breakpad
TARGET = kommas
TEMPLATE = app

win32-msvc* {
 QMAKE_LFLAGS_RELEASE += /MAP
 QMAKE_CFLAGS_RELEASE += /Zi
 QMAKE_LFLAGS_RELEASE += /debug /opt:ref
}
INCLUDEPATH += D:\masteraplikasi\kul\skripsi\baru\qca\include\QtCrypto D:\masteraplikasi\kul\skripsi\baru\qca\include
LIBS += D:\masteraplikasi\stuff\projectall\kommas\qca\qca.lib
win32:HEADERS += $$BREAKPAD/src/common/windows/string_utils-inl.h
win32:HEADERS += $$BREAKPAD/src/common/windows/guid_string.h
win32:HEADERS += $$BREAKPAD/src/client/windows/handler/exception_handler.h
win32:HEADERS += $$BREAKPAD/src/client/windows/common/ipc_protocol.h
win32:HEADERS += $$BREAKPAD/src/google_breakpad/common/minidump_format.h
win32:HEADERS += $$BREAKPAD/src/google_breakpad/common/breakpad_types.h
win32:HEADERS += $$BREAKPAD/src/client/windows/crash_generation/crash_generation_client.h
win32:HEADERS += $$BREAKPAD/src/common/scoped_ptr.h
win32:SOURCES += $$BREAKPAD/src/client/windows/handler/exception_handler.cc
win32:SOURCES += $$BREAKPAD/src/common/windows/string_utils.cc
win32:SOURCES += $$BREAKPAD/src/common/windows/guid_string.cc
win32:SOURCES += $$BREAKPAD/src/client/windows/crash_generation/crash_generation_client.cc
INCLUDEPATH += $$BREAKPAD $$BREAKPAD/src $$PWD
DEPENDPATH += $$BREAKPAD $$BREAKPAD/src

SOURCES += main.cpp crash_handler.cpp \
        mainwindow.cpp \
    encryptwizard.cpp \
    decryptwizard.cpp \
    historyencrypt.cpp \
    historydecrypt.cpp

HEADERS  += mainwindow.h crash_handler.h \
    encryptwizard.h \
    decryptwizard.h \
    historyencrypt.h \
    historydecrypt.h

FORMS    += mainwindow.ui \
    historyencrypt.ui \
    historydecrypt.ui

RESOURCES += \
    res.qrc
