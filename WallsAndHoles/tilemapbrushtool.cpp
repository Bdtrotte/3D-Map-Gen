#include "tilemapbrushtool.h"

#include <QSpinBox>
#include <QLabel>

TileMapBrushTool::TileMapBrushTool(TileMapPreviewGraphicsItem *previewItem)
    : AbstractTileMapTool(previewItem) {}

void TileMapBrushTool::cellActivated(int x, int y)
{
    getTileMap()->setTile(x, y, getTileTemplate());
}

void TileMapBrushTool::cellHovered(int x, int y)
{
    if (TileTemplate *t = getTileTemplate()) {
        mPreviewItem->setRegion(QRect(x, y, 1, 1));
        mPreviewItem->setColor(t->color());
    } else {
        mPreviewItem->setRegion(QRegion());
    }
}

void TileMapBrushTool::mouseExitedMap()
{
    mPreviewItem->setRegion(QRegion());
}

QToolBar *TileMapBrushTool::contextActions()
{
    QToolBar *context = new QToolBar();
    QSpinBox *spinBox = new QSpinBox(context);
    QLabel *test = new QLabel("Brush Tool Test: ");

    context->addWidget(test);
    context->addWidget(spinBox);

    return context;
}


