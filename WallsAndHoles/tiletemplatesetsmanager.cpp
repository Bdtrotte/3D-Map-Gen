#include "tiletemplatesetsmanager.h"

#include "newtiletemplatesetdialog.h"
#include "xmltool.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

TileTemplateSetsManager::TileTemplateSetsManager(TileMap *tileMap,
                                                 QObject *parent)
    : QObject(parent)
    , mTileMap(tileMap) {}

void TileTemplateSetsManager::newTileTemplateSet()
{
    NewTileTemplateSetDialog dia;
    if (dia.exec()) {
        for (SavableTileTemplateSet *ts : mTileTemplateSets) {
            if (dia.result.fileLocation == ts->savePath()) {
                QMessageBox mb;
                mb.setText(tr("Tile Template Set already open at requested location."));
                mb.exec();
                return;
            }
        }

        SavableTileTemplateSet *newTTS = new SavableTileTemplateSet(dia.result.fileLocation, dia.result.name);
        addTileTemplateSet(newTTS);
    }
}

void TileTemplateSetsManager::addTileTemplateSet(SavableTileTemplateSet *tileTemplateSet)
{
    tileTemplateSet->setParent(this);

    mTileTemplateSets.append(tileTemplateSet);

    emit tileTemplateSetAdded(tileTemplateSet);
}

bool TileTemplateSetsManager::removeTileTemplateSet(SavableTileTemplateSet *tileTemplateSet)
{
    int index = mTileTemplateSets.indexOf(tileTemplateSet);

    //should not be called if the passed template set is not part of the manager.
    Q_ASSERT(index != -1);

    return removeTileTemplateSet(index);
}

bool TileTemplateSetsManager::removeTileTemplateSet(int index)
{
    SavableTileTemplateSet *tileTemplateSet = mTileTemplateSets[index];

    bool removeFromMap = false;

    //if this template set is in use with the tileMap
    if (mTileMap && mTileMap->tileTemplateSetUsed(tileTemplateSet)) {
        //Prompt user saying the set is in use, and if it is removed,
        //then the tiles using templates from this set will be reset
        // TODO: They should have an option to replace with another template?

        QMessageBox mb;
        mb.setText("The Tile Template Set you are attempting to remove is "
                   "being used by the active Tile Map. Removing this Template Set will "
                   "reset any tiles using templates from this set back to the "
                   "default.\nDo you wish to proceed?");
        mb.addButton("Remove", QMessageBox::AcceptRole);
        mb.addButton("Cancel", QMessageBox::RejectRole);

        //if the user presses cancel
        if (mb.exec() == 1)
            return false;

        //at this point, the set is going to be removed, and the attached tiles need to be cleared.
        removeFromMap = true;
    }

    //if the templateSet is not saved, prompt the user if they want to save it.
    if (!tileTemplateSet->isSaved()) {
        QMessageBox mb;
        mb.setText("Save Tile Template Set before removing?");
        mb.addButton("Save", QMessageBox::YesRole);
        mb.addButton("Don't Save", QMessageBox::NoRole);
        mb.addButton("Cancel", QMessageBox::RejectRole);

        switch(mb.exec()) {
        case 0:
            tileTemplateSet->save();
            break;
        case 2:
            return false;
        default: break;
        }
    }

    emit tileTemplateSetAboutToBeRemoved(tileTemplateSet);

    if (removeFromMap)
        mTileMap->removingTileTemplateSet(tileTemplateSet);

    mTileTemplateSets.removeAt(index);

    return true;
}

void TileTemplateSetsManager::saveAllTileTemplateSets()
{
    for (SavableTileTemplateSet *ts : mTileTemplateSets)
        ts->save();
}

SavableTileTemplateSet *TileTemplateSetsManager::loadTileTemplateSet(bool tryToRelocateOnFail)
{
    QString path = QFileDialog::getOpenFileName(nullptr,
                                                tr("Load Tile Template Set"),
                                                "/home/",
                                                tr("WTS files (*.wts)"));

    return loadTileTemplateSet(path, tryToRelocateOnFail);
}

SavableTileTemplateSet *TileTemplateSetsManager::loadTileTemplateSet(QString path, bool tryToRelocateOnFail)
{
    for (SavableTileTemplateSet *ts : mTileTemplateSets)
        if (path == ts->savePath())
            return ts;

    if (!path.isNull()) {
        SavableTileTemplateSet *newSet = XMLTool::openTileTemplateSet(path);
        if (newSet != nullptr) {
            addTileTemplateSet(newSet);

            return newSet;
        } else if (tryToRelocateOnFail) {
            QMessageBox mb;
            mb.setText(QString("Failed to load Tile Template Set at:\n    %1\n"
                               "Would you like to try to relocate the Tile Template Set?\n").arg(path));
            mb.addButton("Relocate", QMessageBox::AcceptRole);
            mb.addButton("Cancel", QMessageBox::RejectRole);

            if (mb.exec() == 0) {
                //relocate
                return loadTileTemplateSet(true);
            }
        }
    }

    return nullptr;
}
