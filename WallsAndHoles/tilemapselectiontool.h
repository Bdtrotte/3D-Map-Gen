#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H

#define MULTI_CLICK_TIME 200

#include "abstracttileselectiontool.h"

class TileMapSelectionTool : public AbstractTileSelectionTool
{
public:
    TileMapSelectionTool(PropertyBrowser *propertyBrowser,
                         TileMapPreviewGraphicsItem *previewItem,
                         QObject *parent = nullptr);

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
};

#endif // TILEMAPSELECTIONTOOL_H
