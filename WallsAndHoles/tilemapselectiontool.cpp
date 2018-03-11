#include "tilemapselectiontool.h"

#include "shaperegion.h"
#include "tilemaphelpers.h"
#include "tilepropertymanager.h"

#include <QDebug>

TileMapSelectionTool::TileMapSelectionTool(PropertyBrowser *propertyBrowser,
                                           TileMapPreviewGraphicsItem *previewItem,
                                           QUndoStack *undoStack)
    : AbstractTileSelectionTool(propertyBrowser, previewItem)
    , mClickCount(0)
    , mLastClickTime(0)
    , mUndoStack(undoStack) {}

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
    if (mCurrentRect.boundingRect().width() == 1 && mCurrentRect.boundingRect().height() == 1) {
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
        newSelectionRegion = TileMapHelper::getFillRegion(getTileMap(), mCurrentRect.boundingRect().left(), mCurrentRect.boundingRect().top());
        break;
    case 2:
        newSelectionRegion = TileMapHelper::getAllOfTemplateAtTile(getTileMap(), mCurrentRect.boundingRect().left(), mCurrentRect.boundingRect().top());
        break;
    }

    QRegion newSelectionValue;
    switch (event->modifiers()) {
    case Qt::ShiftModifier:
        newSelectionValue = mOriginalSelection + newSelectionRegion;
        break;
    case Qt::ControlModifier:
        newSelectionValue = mOriginalSelection - newSelectionRegion;
        break;
    case Qt::ShiftModifier | Qt::ControlModifier:
        newSelectionValue = mOriginalSelection & newSelectionRegion;
        break;
    case Qt::NoModifier:
        newSelectionValue = newSelectionRegion;
        break;
    }

    QRegion originalSelection = mOriginalSelection;
    mUndoStack->push(SelectionChangeCommand::make(
                             &mSelection,
                             newSelectionValue,
                             "'changed selection'",
                             [this, originalSelection] () {
        // This makes the behavior more intuitive if the user does an undo in the middle of selecting.
        // (note: the user's current preview of the in-progress selection will disappear until the user
        //  moves their cursor---I couldn't find a neat way of avoiding this, but it doesn't cause
        //  too much trouble)
        mOriginalSelection = originalSelection;
        activateSelection();
    }));
}

void TileMapSelectionTool::updatePreview(QPoint end)
{
    mCurrentRect = ShapeRegion::rect(mStartPoint, end) & QRect(QPoint(0, 0), getTileMap()->mapSize());

    drawPreview(mOriginalSelection + mCurrentRect);
}
