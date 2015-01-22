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
    src/map.cpp \
    src/tileset.cpp \
    src/ui/newtilesetdialog.cpp \
    src/ui/questdatabase.cpp \
    src/tilesetview.cpp \
    src/ui/mission/editkeyevent.cpp \
    src/mission/key.cpp \
    src/mission/gate.cpp \
    src/mission/mission.cpp \
    src/applicationdispatcher.cpp \
    src/ui/solarusdirectorydialog.cpp \
    src/preferences.cpp \
    src/ui/mission/editgatedialog.cpp \
    src/mission/stage.cpp \
    src/ui/mission/missionstructurescene.cpp \
    src/space/space.cpp \
    src/space/zone.cpp \
    src/ui/space/editzonedialog.cpp \
    src/ui/space/spacescene.cpp \
    src/space/area.cpp \
    src/space/link.cpp \
    src/ui/space/spaceview.cpp \
    src/entities/door.cpp \
    src/entities/mapentity.cpp \
    src/entities/teletransporter.cpp \
    src/qpointext.cpp \
    src/mapscript.cpp \
    src/entities/switchentity.cpp

HEADERS  += \
    include/common.h \
    include/quest.h \
    include/filetools.h \
    include/ui/editorwindow.h \
    include/ui/newquestdialog.h \
    include/ui/openquestdialog.h \
    include/map.h \
    include/tileset.h \
    include/ui/newtilesetdialog.h \
    include/ui/questdatabase.h \
    include/tilesetview.h \
    include/ui/mission/editkeyevent.h \
    include/mission/key.h \
    include/mission/gate.h \
    include/mission/mission.h \
    include/applicationdispatcher.h \
    include/ui/solarusdirectorydialog.h \
    include/preferences.h \
    include/ui/mission/editgatedialog.h \
    include/mission/stage.h \
    include/ui/mission/missionstructurescene.h \
    include/space/space.h \
    include/space/zone.h \
    include/ui/space/editzonedialog.h \
    include/ui/space/spacescene.h \
    include/space/area.h \
    include/space/link.h \
    include/ui/space/spaceview.h \
    include/entities/door.h \
    include/entities/mapentity.h \
    include/entities/teletransporter.h \
    include/qpointext.h \
    include/mapscript.h \
    include/entities/switchentity.h

FORMS    += \
    ui/editorwindow.ui \
    ui/newquestdialog.ui \
    ui/openquestdialog.ui \
    ui/newtilesetdialog.ui \
    ui/questdatabase.ui \
    ui/mission/editkeyevent.ui \
    ui/solarusdirectorydialog.ui \
    ui/mission/editgatedialog.ui \
    ui/space/editzonedialog.ui

INCLUDEPATH += include \
               include/ui \
               include/ui/mission \
               include/ui/space \
               include/mission \
               include/space \
               include/entities \

# Copy Data & Script Files to build
data.path = $${OUT_PWD}/game_data
data.files += game_data/*
INSTALLS += data
