#-------------------------------------------------
#
# Project created by QtCreator 2013-12-19T18:00:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SampleManager
TEMPLATE = app

CONFIG(debug, debug|release) {
        MOC_DIR = ./
}

CONFIG(release, debug|release) {
        MOC_DIR = ./
}


SOURCES += main.cpp\
        CMainWindow.cpp

HEADERS  += CMainWindow.h

FORMS    += cmainwindow.ui
