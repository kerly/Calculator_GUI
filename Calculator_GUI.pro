#-------------------------------------------------
#
# Project created by QtCreator 2013-11-30T19:35:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calculator_GUI
TEMPLATE = app

CONFIG += app_bundle


SOURCES += main.cpp\
        mainwindow.cpp\
        calculatorfunctions.cpp

HEADERS  += mainwindow.h\
         calculatorfunctions.h

FORMS    += mainwindow.ui

OTHER_FILES +=

ParentDirectory = "P:\HDD Documents\Programming Workspace\QT\My Own\Calculator_GUI"

RCC_DIR = "$$ParentDirectory\Build\RCCFiles"
UI_DIR = "$$ParentDirectory\Build\UICFiles"
MOC_DIR = "$$ParentDirectory\Build\MOCFiles"
OBJECTS_DIR = "$$ParentDirectory\Build\ObjFiles"

CONFIG(debug, debug|release) {
    DESTDIR = "$$ParentDirectory\debug"
}
CONFIG(release, debug|release) {
    DESTDIR = "$$ParentDirectory\release"
}
