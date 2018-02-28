#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H

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
    QRect mCurrentRect;
    QRegion mOriginalSelection;
};

#endif // TILEMAPSELECTIONTOOL_H
