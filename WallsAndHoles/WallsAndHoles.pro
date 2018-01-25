#-------------------------------------------------
#
# Project created by QtCreator 2018-01-13T22:09:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WallsAndHoles
TEMPLATE = app

CONFIG += c++14


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    meshview.cpp \
    meshviewcameralikeblender.cpp \
    renderableobject.cpp \
    scene.cpp \
    tile.cpp \
    tilemap.cpp \
    tiletemplate.cpp \
    tiletemplateset.cpp \
    drawableaxes.cpp \
    toolmanager.cpp


HEADERS += \
    mainwindow.h \
    renderableobject.h \
    scene.h \
    meshview.h \
    meshviewcameralikeblender.h \
    objtools.h \
    drawableaxes.h \
    toolmanager.h \
    renderableobject.h \
    scene.h \
    tile.h \
    tilemap.h \
    tiletemplate.h \
    tiletemplateset.h \
    abstracttool.h \
    abstractmeshviewcamera.h \
    abstractdrawableglobject.h

FORMS += \
    mainwindow.ui \
    meshview.ui

DISTFILES += \
    basic.fsh \
    basic.vsh

RESOURCES += \
    shaders.qrc
