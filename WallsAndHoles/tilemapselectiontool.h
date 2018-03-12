#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H

#define MULTI_CLICK_TIME 200

#include "abstracttileselectiontool.h"
#include "changevaluecommand.h"

class TileMapSelectionTool : public AbstractTileSelectionTool
{
public:
    TileMapSelectionTool(PropertyBrowser *propertyBrowser,
                         TileMapPreviewGraphicsItem *previewItem,
                         QUndoStack *undoStack);

    void cellClicked(int x, int y, QMouseEvent *) override;
    void cellActivated(int x, int y, QMouseEvent *) override;
    void cellReleased(int, int, QMouseEvent *event) override;

private:
    void updatePreview(QPoint end);

    QPoint mStartPoint;
    QRegion mCurrentRect;
    QRegion mOriginalSelection;

    int mClickCount;
    ulong mLastClickTime;

    QUndoStack *mUndoStack;

    using SelectionChangeCommand = ChangeValueCommand<QRegion>;
};

#endif // TILEMAPSELECTIONTOOL_H
