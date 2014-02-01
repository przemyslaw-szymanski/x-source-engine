#-------------------------------------------------
#
# Project created by QtCreator 2013-12-19T18:00:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SampleManager
TEMPLATE = app

QMAKE_CXXFLAGS += -std=gnu++0x

#INCLUDEPATH += ../../../../Engine/include \
#            ../../

CONFIG(debug, debug|release) {
        MOC_DIR = ./
}

CONFIG(release, debug|release) {
        MOC_DIR = ./
}


SOURCES += main.cpp\
        CMainWindow.cpp \
    cdebugdialog.cpp

HEADERS  += CMainWindow.h \
    cdebugdialog.h

FORMS    += cmainwindow.ui \
    cdebugdialog.ui
