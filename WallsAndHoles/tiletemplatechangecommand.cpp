#include "tiletemplatechangecommand.h"

TileTemplateChangeCommand *TileTemplateChangeCommand::performCommand(
        TileMap *tileMap,
        QRegion changedPoints,
        TileTemplate *newTileTemplate,
        const QString &text)
{
    // Compute which points need to be changed. Make sure all points are within bounds.
    QVector<QPoint> pointsToChange;
    for (const QRect &rect : changedPoints)
        for (int x = rect.left(); x <= rect.right(); ++x)
            for (int y = rect.top(); y <= rect.bottom(); ++y)
                if (tileMap->contains(x, y))
                    pointsToChange.append(QPoint(x, y));

    // Fetch the old templates for the tiles.
    QVector<TileTemplate *> oldTemplates;
    for (const QPoint &pt : pointsToChange)
        oldTemplates.append(tileMap->tileAt(pt.x(), pt.y()).tileTemplate());

    // Create the command.
    TileTemplateChangeCommand *command = new TileTemplateChangeCommand(tileMap, pointsToChange, oldTemplates, newTileTemplate, text);

    // Perform the command.
    command->redo();

    return command;
}


TileTemplateChangeCommand::TileTemplateChangeCommand(
        TileMap *tileMap,
        QVector<QPoint> changedPositions,
        QVector<TileTemplate *> oldTemplates,
        TileTemplate *newTemplate,
        const QString &text)
    : QUndoCommand(text)
    , mTileMap(tileMap)
    , mChangedTilePositions(changedPositions)
    , mOldTemplatePointers(oldTemplates)
    , mNewTemplatePointer(newTemplate) {}


void TileTemplateChangeCommand::undo()
{
    for (int idx = 0; idx < mChangedTilePositions.size(); ++idx) {
        QPoint pos = mChangedTilePositions[idx];
        TileTemplate *oldTemplate = mOldTemplatePointers[idx];

        mTileMap->setTile(pos.x(), pos.y(), oldTemplate);
    }
}


void TileTemplateChangeCommand::redo()
{
    for (const QPoint &pos : mChangedTilePositions)
        mTileMap->setTile(pos.x(), pos.y(), mNewTemplatePointer);
}
