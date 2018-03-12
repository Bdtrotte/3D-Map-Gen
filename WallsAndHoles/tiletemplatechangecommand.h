#ifndef TILEMAPUNDOCOMMAND_H
#define TILEMAPUNDOCOMMAND_H


#include "tilemap.h"

#include <QUndoCommand>
#include <QRegion>
#include <QObject>

class TileTemplateChangeCommand : public QObject, public QUndoCommand
{

    Q_OBJECT

public:

    /**
     * @brief make              Creates the TileTemplateChangeCommand.
     *                          This will automatically crop the given region so that it is within the bounds of the TileMap.
     *
     * @param tileMap           The tile map on which to change tiles.
     * @param changedPoints     The positions of the tiles that should be changed.
     * @param newTileTemplate   The new tile template for those tiles.
     * @param text              Short description for the command.
     * @param parent            The parent command.
     * @return                  The command that was performed. Calling undo() will undo it.
     */
    static TileTemplateChangeCommand *make(TileMap *tileMap,
            QRegion changedPoints,
            TileTemplate *newTileTemplate,
            const QString &text = "Changed templates for tiles.",
            QUndoCommand *parent = nullptr,
            int id = -1,
            bool canMerge = false);


    /**
     * @brief make              Creates the TileTemplateChangeCommand.
     *                          This will automatically crop the given region so that it is within the bounds of the TileMap.
     *
     * @param tileMap           The tile map on which to change tiles.
     * @param changedPoints     The positions of the tiles that should be changed.
     * @param newTileTemplate   The new tile template for those tiles.
     * @param text              Short description for the command.
     * @param parent            The parent command.
     * @return                  The command that was performed. Calling undo() will undo it.
     */
    template< typename QPointIterable >
    static TileTemplateChangeCommand *make(
            TileMap *tileMap,
            QPointIterable changedPoints,
            TileTemplate *newTileTemplate,
            const QString &text = "Changed templates for tiles.",
            QUndoCommand *parent = nullptr)
    {
        QRegion region;
        for (const QPoint &pt : changedPoints)
            region += QRect(pt.x(), pt.y(), 1, 1);
        return make(tileMap, region, newTileTemplate, text, parent);
    }


    void undo() override;
    void redo() override;

    int id() const override { return mId; }

    bool mergeWith(const QUndoCommand *other);

private:

    TileTemplateChangeCommand(TileMap *tileMap,
                              QVector<QPoint> changedPositions,
                              QVector<TileTemplate *> oldTemplates,
                              TileTemplate *newTemplate,
                              const QString &text,
                              QUndoCommand *parent,
                              int id = -1,
                              bool canMerge = false);

    TileMap *mTileMap;                              // If mTileMap becomes invalid, the TileMap undo stack will be cleared.

    QVector<QPoint> mChangedTilePositions;
    QVector<TileTemplate *> mOldTemplatePointers;
    TileTemplate *mNewTemplatePointer;

    int mId;
    bool mCanMerge;
};

#endif // TILEMAPUNDOCOMMAND_H
