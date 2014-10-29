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
    src/editorwindow.cpp \
    src/newquestdialog.cpp \
    src/quest.cpp \
    src/openquestdialog.cpp

HEADERS  += \
    include/editorwindow.h \
    include/common.h \
    include/newquestdialog.h \
    include/quest.h \
    include/openquestdialog.h

FORMS    += \
    ui/editorwindow.ui \
    ui/newquestdialog.ui \
    ui/openquestdialog.ui

INCLUDEPATH += ui
INCLUDEPATH += src
INCLUDEPATH += include

# Copy Data & Script Files to build
data.path = $${OUT_PWD}/game_data
data.files += game_data/*
INSTALLS += data
