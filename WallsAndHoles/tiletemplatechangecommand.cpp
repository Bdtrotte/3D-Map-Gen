#include "tiletemplatechangecommand.h"

#include <QSet>

TileTemplateChangeCommand *TileTemplateChangeCommand::make(
        TileMap *tileMap,
        QRegion changedPoints,
        TileTemplate *newTileTemplate,
        const QString &text,
        QUndoCommand *parent)
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
    return new TileTemplateChangeCommand(tileMap, pointsToChange, oldTemplates, newTileTemplate, text, parent);
}


TileTemplateChangeCommand::TileTemplateChangeCommand(
        TileMap *tileMap,
        QVector<QPoint> changedPositions,
        QVector<TileTemplate *> oldTemplates,
        TileTemplate *newTemplate,
        const QString &text,
        QUndoCommand *parent)
    : QUndoCommand(text, parent)
    , mTileMap(tileMap)
    , mChangedTilePositions(changedPositions)
    , mOldTemplatePointers(oldTemplates)
    , mNewTemplatePointer(newTemplate)
{
    auto makeObsolete = [this] () {
        if (isObsolete())
            return;

        setObsolete(true);
        qDebug() << "The command " << this->text() << " is obsolete because a pointer it referenced is no longer valid.";

        /*
         * Note that this lambda gets invoked whenever mTileMap or any of the tile template objects are destroyed.
         * When the application closes, these objects may be destroyed before the undo commands, so if you see
         * 'The command ... is obsolete ...' messages when you close the program, do not worry.
         * */
    };


    // If the TileMap pointer is invalid, this command should not be performable.
    connect(mTileMap, &TileMap::destroyed, this, makeObsolete);


    // If a TileTemplate pointer is invalid, this command should not be performable.
    // To avoid connecting several times to the same template, all the TileTemplate*
    // are first collected into a set.
    QSet<TileTemplate *> allTemplatePointers;

    for (TileTemplate *p : oldTemplates) {
        if (p != nullptr)
            allTemplatePointers.insert(p);
    }

    if (newTemplate != nullptr)
        allTemplatePointers.insert(newTemplate);


    for (TileTemplate *p : allTemplatePointers)
        connect(p, &TileTemplate::destroyed, this, makeObsolete);
}


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
