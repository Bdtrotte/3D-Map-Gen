#include "tiletemplatesetsmanager.h"

#include "newtiletemplatesetdialog.h"
#include "xmltool.h"

#include "undocommandfromfunctions.h"
#include "dependentundocommand.h"
#include "tiletemplatechangecommand.h"
#include "emptyparentcommand.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QRegion>
#include <QRect>

TileTemplateSetsManager::TileTemplateSetsManager(QUndoStack *undoStack,
                                                 TileMap *tileMap,
                                                 QObject *parent)
    : QObject(parent)
    , mTileMap(tileMap)
    , mUndoStack(undoStack) {}

void TileTemplateSetsManager::newTileTemplateSet()
{
    NewTileTemplateSetDialog dia;
    if (dia.exec()) {
        for (SavableTileTemplateSet *ts : mTileTemplateSets) {
            if (dia.result().fileLocation == ts->savePath()) {
                QMessageBox mb;
                mb.setText(tr("Tile Template Set already open at requested location."));
                mb.exec();
                return;
            }
        }

        SavableTileTemplateSet *newTTS = new SavableTileTemplateSet(dia.result().fileLocation, dia.result().name);
        addTileTemplateSet(newTTS);
    }
}

void TileTemplateSetsManager::addTileTemplateSet(SavableTileTemplateSet *tileTemplateSet)
{

    auto redo = [this, tileTemplateSet] () {
        tileTemplateSet->setParent(this);
        mTileTemplateSets.append(tileTemplateSet);
        emit tileTemplateSetAdded(tileTemplateSet);
    };

    auto undo = [this, tileTemplateSet] () {
        // Do essentially the same thing as in removeTileTemplateSet()
        // under the assumption that no tiles are using this set.
        int index = mTileTemplateSets.indexOf(tileTemplateSet);

        Q_ASSERT( index != -1 );

        emit tileTemplateSetAboutToBeRemoved(tileTemplateSet);
        mTileTemplateSets.removeAt(index);
    };

    QUndoCommand *command = DependentUndoCommand::make({this, tileTemplateSet}, "'add tile template set'");

    UndoCommandFromFunctions::make(redo, undo, "", command);

    mUndoStack->push(command);
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
    if (mTileMap && mTileMap->isTileTemplateSetUsed(tileTemplateSet)) {
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


    QUndoCommand *command = EmptyParentCommand::make("'remove tile template set'");


    // When the command is redone, it should emit this signal.
    UndoCommandFromFunctions::make([this, tileTemplateSet] () {
        emit tileTemplateSetAboutToBeRemoved(tileTemplateSet);
    }, [](){}, "", DependentUndoCommand::make({this, tileTemplateSet}, "", command));


    if (removeFromMap) {
        // Create the command to clear the tiles.

        QVector<QPoint> allPoints = mTileMap->tilePositionsUsingTemplateSet(tileTemplateSet);

        TileTemplateChangeCommand::make(mTileMap, allPoints, nullptr, "'clear tiles that use template set'", command);
    }


    // Create the command to remove tileTemplateSet from the list.
    auto removeRedo = [this, tileTemplateSet] () {

        // This will work even if the sets are rearranged for some reason.
        int index = mTileTemplateSets.indexOf(tileTemplateSet);

        Q_ASSERT( index != -1 );

        mTileTemplateSets.removeAt(index);
    };

    auto removeUndo = [this, tileTemplateSet] () {
        // Do essentially the same thing as in addTileTemplateSet()
        tileTemplateSet->setParent(this);
        mTileTemplateSets.append(tileTemplateSet);
        emit tileTemplateSetAdded(tileTemplateSet);
    };

    UndoCommandFromFunctions::make(removeRedo, removeUndo, "",
                                   DependentUndoCommand::make({this, tileTemplateSet}, "'remove tile template set'", command));

    // Push and perform the command.
    mUndoStack->push(command);

    return true;
}

bool TileTemplateSetsManager::removeTileTemplate(int templateSetIndex, int templateIndex)
{
    Q_ASSERT(templateSetIndex >= 0 && templateSetIndex < mTileTemplateSets.size());
    Q_ASSERT(templateIndex >= 0 && templateIndex < mTileTemplateSets[templateSetIndex]->size());

    auto tileTemplateSet = mTileTemplateSets[templateSetIndex];

    TileTemplate *t = tileTemplateSet->tileTemplateAt(templateIndex);

    QUndoCommand *command = EmptyParentCommand::make("'remove tile template'");

    if (mTileMap && mTileMap->isTileTemplateUsed(t)) {
        QMessageBox mb;
        mb.setText("The Tile Template you are attempting to remove is "
                   "being used by the active Tile Map. Removing this Template will "
                   "reset any tiles using this template back to the "
                   "default.\nDo you wish to proceed?");
        mb.addButton("Remove", QMessageBox::AcceptRole);
        mb.addButton("Cancel", QMessageBox::RejectRole);

        if (mb.exec() == 1) return false;


        QVector<QPoint> tilePositions = mTileMap->tilePositionsUsingTemplate(t);

        // Command to clear tiles.
        TileTemplateChangeCommand::make(mTileMap, tilePositions, nullptr, "'clear tiles that use template'", command);
    }


    // Command to remove the tile template.

    auto removeRedo = [this, tileTemplateSet, templateIndex] () {
        tileTemplateSet->removeTileTemplate(templateIndex);
    };

    auto removeUndo = [this, tileTemplateSet, t] () {
        tileTemplateSet->addTileTemplate(t);
    };

    UndoCommandFromFunctions::make(removeRedo, removeUndo, "", DependentUndoCommand::make({this, tileTemplateSet, t}, "", command));


    mUndoStack->push(command);

    return true;
}

void TileTemplateSetsManager::addTileTemplate(int templateSetIndex, TileTemplate *newTemplate)
{
    Q_ASSERT(templateSetIndex >= 0 && templateSetIndex < mTileTemplateSets.size());

    /*
     * It is assumed that when a template is added, it is added to the end of the TileTemplateSet's list.
     * Therefore, the undo command simply removes the element at the end.
     *
     * If newTemplate is destroyed, the command becomes obsolete: this is ensured by the DependentUndoCommand.
     * */

    auto tileTemplateSet = tileTemplateSetAt(templateSetIndex);

    auto redo = [this, tileTemplateSet, newTemplate] () {
        tileTemplateSet->addTileTemplate(newTemplate);
    };

    auto undo = [this, tileTemplateSet] () {
        tileTemplateSet->removeTileTemplate(tileTemplateSet->size() - 1);
    };

    QString name = tr("'add tile template'");

    auto command = DependentUndoCommand::make({this, newTemplate, tileTemplateSet}, name);

    UndoCommandFromFunctions::make(
                     redo,
                     undo,
                     name,
                     command);

    mUndoStack->push(command);
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
