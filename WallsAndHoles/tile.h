#ifndef TILE_H
#define TILE_H

#include "tiletemplate.h"

#include <QObject>
#include <QVector2D>

/**
 * @brief Class to represent each tile on the tileMap
 */
class Tile : public QObject
{
    Q_OBJECT

public:
    explicit Tile(const SharedTileTemplate tileTemplate,
                  int xPos = -1,
                  int yPos = -1,
                  QObject *parent = nullptr);

    const SharedTileTemplate tileTemplate() const { return mTileTemplate; }

    float relativeThickness() const { return mRelativeThickness; }
    float relativeHeight() const { return mRelativeHeight; }
    QVector2D relativePosition() const { return mRelativePosition; }

    //will be clipped such that 0 < mTileTemplate.thickness() + relativeThickness <= 1
    //and so walls don't leave tile bounds
    void setRelativeThickness(float relativeThickness);
    void setRelativeHeight(float relativeHeight);
    //will be clipped so that walls don't leave tilebounds
    void setRelativePosition(QVector2D relavtivePosition);

signals:
    void tileChanged(int x, int y);

private:
    const SharedTileTemplate mTileTemplate;

    const int mXPos;
    const int mYPos;

    //The thickness of the tile relative to the base tile types thickness:
    //If tileId 1 has base thickness 0.5, and mTileId = 1 and mRelativeSize = 0.2,
    //then this tile will have thickness 0.7. The total should not exceed 1 or be
    //equal to or less than 0
    float mRelativeThickness;

    //The height of the tile relative to the base tile types thickness:
    //As with above, this is added to the base tileHeights height
    float mRelativeHeight;

    //let x = (1 - mSize) / 2;
    //then both x and y are between -x and x (inclusive)
    //This is the offset of the tile relative to the center of it's grid position
    //(0,0 would be the centered regardless of the tiles coordinates)
    QVector2D mRelativePosition;

    //other properties to come as we develop further
};

#endif // TILE_H
