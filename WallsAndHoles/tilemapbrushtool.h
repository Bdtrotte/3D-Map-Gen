#ifndef TILEMAPBRUSHTOOL_H
#define TILEMAPBRUSHTOOL_H

#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "tiletemplate.h"

#include <QWidgetAction>
#include <QUndoStack>

class TileMapBrushTool : public AbstractTileMapTool
{
public:
    TileMapBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack);

    void cellActivated(int x, int y, QMouseEvent *) override;
    void cellHovered(int x, int y, QMouseEvent *) override;
    void mouseExitedMap(QMouseEvent *);

    QList<QAction *> contextActions() override { return {mRadiusSpinner}; }

private:
    int mRadius;

    QWidgetAction *mRadiusSpinner;

    QUndoStack *mUndoStack;
};

#endif // TILEMAPBRUSHTOOL_H
