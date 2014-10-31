#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T16:53:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProcLevelDesigner
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    src/main.cpp \
    src/quest.cpp \
    src/filetools.cpp \
    src/ui/editorwindow.cpp \
    src/ui/newquestdialog.cpp \
    src/ui/openquestdialog.cpp \
    src/ui/questoptions.cpp

HEADERS  += \
    include/common.h \
    include/quest.h \
    include/filetools.h \
    include/ui/editorwindow.h \
    include/ui/newquestdialog.h \
    include/ui/openquestdialog.h \
    include/ui/questoptions.h

FORMS    += \
    ui/editorwindow.ui \
    ui/newquestdialog.ui \
    ui/openquestdialog.ui \
    ui/questoptions.ui

INCLUDEPATH += include \
               include/ui

# Copy Data & Script Files to build
data.path = $${OUT_PWD}/game_data
data.files += game_data/*
INSTALLS += data
