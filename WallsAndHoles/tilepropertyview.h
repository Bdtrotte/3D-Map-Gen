#ifndef TILEPROPERTYVIEW_H
#define TILEPROPERTYVIEW_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>

/**
 * @brief TilePropertyView display the property of Tile.
 * Whenever a tile is selected, it automatically pop out.
 * It blanks out when mutiple tiles are selected.
 */
class TilePropertyView : public QWidget
{
public:
    TilePropertyView(QWidget *parent = nullptr);
};

#endif // TILEPROPERTYVIEW_H
