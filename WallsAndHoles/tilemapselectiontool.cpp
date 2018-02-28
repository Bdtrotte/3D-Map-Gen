#include "tilemapselectiontool.h"

#include "tilemaphelpers.h"
#include "tilepropertymanager.h"

#include <QDebug>

TileMapSelectionTool::TileMapSelectionTool(PropertyBrowser *propertyBrowser,
                                           TileMapPreviewGraphicsItem *previewItem,
                                           QObject *parent)
    : AbstractTileSelectionTool(propertyBrowser, previewItem, parent)
    , mClickCount(0)
    , mLastClickTime(0) {}

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
    if (mCurrentRect.width() == 1 && mCurrentRect.height() == 1) {
        if (event->timestamp() - mLastClickTime < MULTI_CLICK_TIME)
            ++mClickCount;
        else
            mClickCount = 0;

        mLastClickTime = event->timestamp();
    } else {
        mClickCount = 0;
    }

    if (mClickCount > 2)
        mClickCount = 2;

    QRegion newSelectionRegion;
    switch (mClickCount) {
    case 0:
        newSelectionRegion = mCurrentRect;
        break;
    case 1:
        newSelectionRegion = TileMapHelper::getFillRegion(getTileMap(), mCurrentRect.left(), mCurrentRect.top());
        break;
    case 2:
        newSelectionRegion = TileMapHelper::getAllOfTemplateAtTile(getTileMap(), mCurrentRect.left(), mCurrentRect.top());
        break;
    }

    switch (event->modifiers()) {
    case Qt::ShiftModifier:
        mSelection = mOriginalSelection + newSelectionRegion;
        break;
    case Qt::ControlModifier:
        mSelection = mOriginalSelection - newSelectionRegion;
        break;
    case Qt::ShiftModifier | Qt::ControlModifier:
        mSelection = mOriginalSelection & newSelectionRegion;
        break;
    case Qt::NoModifier:
        mSelection = newSelectionRegion;
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
