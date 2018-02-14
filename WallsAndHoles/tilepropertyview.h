#ifndef TILEPROPERTYVIEW_H
#define TILEPROPERTYVIEW_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "tilemap.h"

/**
 * @brief TilePropertyView display the property of Tile.
 * Whenever a tile is selected, it will be filled out and support modification.
 * It should blanks out when mutiple tiles are selected.
 */
class TilePropertyView : public QWidget
{
    Q_OBJECT

public:
    explicit TilePropertyView(QWidget *parent = nullptr);

    /**
     * @brief set current focusing tile of this view.
     */
    void setTile(Tile& tile);

    void clear();

private slots:
    void relativeHeightChanged(double value);
    void relativeThicknessChanged(double value);
    void relativeXPositionChanged(double value);
    void relativeYPositionChanged(double value);

private:
    //bool mShow;
    QLabel *mRelativeHeightLabel;
    QDoubleSpinBox *mRelativeHeight;
    QLabel *mRelativeThicknessLabel;
    QDoubleSpinBox *mRelativeThickness;
    QLabel *mRelativeXPositionLabel;
    QDoubleSpinBox *mRelativeXPosition;
    QLabel *mRelativeYPositionLabel;
    QDoubleSpinBox *mRelativeYPosition;

    Tile *mTile;
};

#endif // TILEPROPERTYVIEW_H
