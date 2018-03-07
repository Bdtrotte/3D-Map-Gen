#ifndef TILEMAPUNDOCOMMAND_H
#define TILEMAPUNDOCOMMAND_H


#include "tilemap.h"

#include <QUndoCommand>
#include <QRegion>

class TileTemplateChangeCommand : public QUndoCommand
{
public:

    /**
     * @brief performCommand    Creates and immediately performs the TileTemplateChangeCommand.
     *                          This will automatically crop the given region so that it is within the bounds of the TileMap.
     *
     * @param tileMap           The tile map on which to change tiles.
     * @param changedPoints     The positions of the tiles that should be changed.
     * @param newTileTemplate   The new tile template for those tiles.
     * @param text              Short description for the command.
     * @return                  The command that was performed. Calling undo() will undo it.
     */
    static TileTemplateChangeCommand *performCommand(
            TileMap *tileMap,
            QRegion changedPoints,
            TileTemplate *newTileTemplate,
            const QString &text = "Changed templates for tiles.");


    void undo() override;
    void redo() override;

private:

    TileTemplateChangeCommand(TileMap *tileMap,
                              QVector<QPoint> changedPositions,
                              QVector<TileTemplate *> oldTemplates,
                              TileTemplate *newTemplate,
                              const QString &text);

    TileMap *mTileMap;

    QVector<QPoint> mChangedTilePositions;
    QVector<TileTemplate *> mOldTemplatePointers;
    TileTemplate *mNewTemplatePointer;
};

#endif // TILEMAPUNDOCOMMAND_H
