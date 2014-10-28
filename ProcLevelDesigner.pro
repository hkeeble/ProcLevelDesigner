#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T16:53:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProcLevelDesigner
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/editorwindow.cpp

HEADERS  += \
    include/editorwindow.h

FORMS    += \
    ui/editorwindow.ui

INCLUDEPATH += ui
INCLUDEPATH += src
INCLUDEPATH += include
