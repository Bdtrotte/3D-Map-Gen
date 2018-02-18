#-------------------------------------------------
#
# Project created by QtCreator 2018-01-13T22:09:54
#
#-------------------------------------------------

QT       += core gui xml

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
    meshview.cpp \
    meshviewcameralikeblender.cpp \
    renderableobject.cpp \
    scene.cpp \
    tile.cpp \
    tilemap.cpp \
    tiletemplate.cpp \
    tiletemplateset.cpp \
    drawableaxes.cpp \
    toolmanager.cpp \
    meshviewcontainer.cpp \
    mapview.cpp \
    editor.cpp \
    meshmaterial.cpp \
    shaderprogramonelight.cpp \
    xmltool.cpp \
    map2mesh.cpp \
    mapcell.cpp \
    tilemapbrushtool.cpp \
    tilemaptoolmanager.cpp \
    m2mtilemesher.cpp \
    m2mpropertyclass.cpp \
    m2mpropertyinstance.cpp \
    m2mpropertyset.cpp \
    map2meshproperties.cpp \
    tiletemplatesetsview.cpp \
    newtiletemplatesetdialog.cpp \
    tiletemplateeditor.cpp \
    mapcellgraphicsitem.cpp \
    abstractshapebrushtool.cpp \
    linebrushtool.cpp \
    rectbrushtool.cpp \
    ellipsebrushtool.cpp \
    tilepropertyview.cpp \
    tilemapselectiontool.cpp \
    filltool.cpp \
    tiletemplatesetsmanager.cpp \
    savabletiletemplateset.cpp \
    newmapdialog.cpp \
    tilemappreviewgraphicsitem.cpp \
    propertybrowser.cpp

HEADERS += \
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
    abstracttool.h \
    abstractmeshviewcamera.h \
    abstractdrawableglobject.h \
    meshviewcontainer.h \
    mapview.h \
    editor.h \
    array2d.h \
    meshmaterial.h \
    shaderprogramonelight.h \
    xmltool.h \
    map2mesh.h \
    m2mtilemesher.h \
    shaderprogramonelight.h \
    mapcell.h \
    tilemaptoolmanager.h \
    abstracttilemaptool.h \
    tilemapbrushtool.h \
    m2mpropertyclass.h \
    m2mpropertyinstance.h \
    m2mpropertyset.h \
    tiletemplateset.h \
    tiletemplatesetsview.h \
    newtiletemplatesetdialog.h \
    tiletemplateeditor.h \
    mapcellgraphicsitem.h \
    abstractshapebrushtool.h \
    linebrushtool.h \
    rectbrushtool.h \
    ellipsebrushtool.h \
    tilepropertyview.h \
    tilemapselectiontool.h \
    filltool.h \
    tiletemplatesetsmanager.h \
    savabletiletemplateset.h \
    newmapdialog.h \
    tilemappreviewgraphicsitem.h \
    propertybrowser.h \
    abstractpropertymanager.h

FORMS += \
    meshview.ui \
    meshviewcontainer.ui

DISTFILES += \
    basic.fsh \
    basic.vsh \
    onelight.vsh \
    onelight.fsh

RESOURCES += \
    shaders.qrc
