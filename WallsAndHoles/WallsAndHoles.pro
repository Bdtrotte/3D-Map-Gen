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
    tile.cpp \
    tilemap.cpp \
    tiletemplate.cpp \
    tiletemplateset.cpp \
    drawableaxes.cpp \
    toolmanager.cpp \
    meshviewcontainer.cpp \
    mapview.cpp \
    editor.cpp \
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
    mapcellgraphicsitem.cpp \
    abstractshapebrushtool.cpp \
    linebrushtool.cpp \
    rectbrushtool.cpp \
    ellipsebrushtool.cpp \
    tilemapselectiontool.cpp \
    filltool.cpp \
    simpletexturedrenderer.cpp \
    simpletexturedscene.cpp \
    simpletexturedobject.cpp \
    simpletexturedshader.cpp \
    tiletemplatesetsmanager.cpp \
    savabletiletemplateset.cpp \
    newmapdialog.cpp \
    tilemappreviewgraphicsitem.cpp \
    objtools.cpp \
    propertybrowser.cpp \
    tilepropertymanager.cpp \
    mappropertymanager.cpp \
    colorpickerbutton.cpp \
    tiletemplatepropertymanager.cpp \
    tilematerial.cpp \
    tilematerialview.cpp \
    tilematerialset.cpp \
    tilematerialselectionbar.cpp \
    materialpropertymanager.cpp \
    imagefinderbar.cpp \
    tilematerialselectiondialog.cpp \
    abstracttileselectiontool.cpp \
    tilemaphelpers.cpp \
    shaperegion.cpp \
    mapviewcontainer.cpp\
    m2mpartialmesh.cpp \
    m2mtilemesher_private.cpp

HEADERS += \
    meshview.h \
    meshviewcameralikeblender.h \
    objtools.h \
    drawableaxes.h \
    toolmanager.h \
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
    mapcellgraphicsitem.h \
    abstractshapebrushtool.h \
    linebrushtool.h \
    rectbrushtool.h \
    ellipsebrushtool.h \
    filltool.h \
    abstractrenderer.h \
    simpletexturedrenderer.h \
    abstractscene.h \
    simpletexturedscene.h \
    dereferencingiterator.h \
    dereferencingconstiterator.h \
    simpletexturedobject.h \
    triplet.h \
    simpletexturedshader.h \
    tilemapselectiontool.h \
    tilemappreviewgraphicsitem.h \
    tiletemplatesetsmanager.h \
    savabletiletemplateset.h \
    newmapdialog.h \
    propertybrowser.h \
    tilepropertymanager.h \
    mappropertymanager.h \
    colorpickerbutton.h \
    tiletemplatepropertymanager.h \
    abstractpropertymanager.h \
    tilematerial.h \
    tilematerialview.h \
    tilematerialset.h \
    tilematerialselectionbar.h \
    materialpropertymanager.h \
    imagefinderbar.h \
    imageandsource.h \
    tilematerialselectiondialog.h \
    abstracttileselectiontool.h \
    tilemaphelpers.h \
    shaperegion.h \
    mapviewcontainer.h \
    m2mpartialmesh.h \
    array2dtools.h \
    array2d_private.h \
    m2mtilemesher_private.h

FORMS +=

DISTFILES += \
    basic.fsh \
    basic.vsh \
    onelight.vsh \
    onelight.fsh

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    icons.qrc

RC_FILE = wah.rc
ICON = images/wahicon.icns
