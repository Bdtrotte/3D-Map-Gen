#include "tilemapselectiontool.h"
#include "tilepropertymanager.h"

TileMapSelectionTool::TileMapSelectionTool(PropertyBrowser *propertyBrowser,
                                           TileMapPreviewGraphicsItem *previewItem,
                                           QObject *parent)
    : AbstractTileSelectionTool(propertyBrowser, previewItem, parent) {}

void TileMapSelectionTool::cellClicked(int x, int y, QMouseEvent *)
{
    mOriginalSelection = AbstractTileSelectionTool::selection();

    mStartPoint = QPoint(x, y);
    updatePreview(QPoint(x, y));
}

void TileMapSelectionTool::cellActivated(int x, int y, QMouseEvent *)
{
    updatePreview(QPoint(x, y));
}

void TileMapSelectionTool::cellReleased(int, int, QMouseEvent *event)
{
    switch (event->modifiers()) {
    case Qt::ShiftModifier:
        mSelection = mOriginalSelection + mCurrentRect;
        break;
    case Qt::ControlModifier:
        mSelection = mOriginalSelection - mCurrentRect;
        break;
    case Qt::ShiftModifier | Qt::ControlModifier:
        mSelection = mOriginalSelection & mCurrentRect;
        break;
    case Qt::NoModifier:
        mSelection = mCurrentRect;
        break;
    }

    activateSelection();
}

void TileMapSelectionTool::updatePreview(QPoint end)
{
    int width = std::abs(end.x() - mStartPoint.x()) + 1;
    int height = std::abs(end.y() - mStartPoint.y()) + 1;
    int left, top;
    if (end.x() > mStartPoint.x())
        left = mStartPoint.x();
    else
        left = end.x();

    if (end.y() > mStartPoint.y())
        top = mStartPoint.y();
    else
        top = end.y();

    mCurrentRect = QRect(left, top, width, height) & QRect(QPoint(0, 0), getTileMap()->mapSize());
    mSelection = mOriginalSelection + mCurrentRect;

    drawPreview();
}
