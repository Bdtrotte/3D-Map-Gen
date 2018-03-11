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
    explicit Tile(TileTemplate *tileTemplate = nullptr,
                  int xPos = -1,
                  int yPos = -1,
                  QObject *parent = nullptr);

    bool hasTileTemplate() const { return mTileTemplate != nullptr; }
    TileTemplate *tileTemplate() const { return mTileTemplate; }

    float thickness() const;
    float height() const;
    QVector2D position() const;

    float relativeThickness() const { return mRelativeThickness; }
    float relativeHeight() const { return mRelativeHeight; }
    QVector2D relativePosition() const { return mRelativePosition; }

    /**
     * @brief setRelativeThickness
     *
     * Tries to set the relative thickness of this tile. Will be clipped such that
     * relativeThickness + template->thickness will be in (0, 1].
     *
     * Will also ensure that the thickness doesn't cause the tile to go out of bounds due to the tiles grid position.
     *
     * Becuase of these, the ultimate value set may differ from that passed, so this returns the value that gets set.
     *
     * @param thickness
     * @return
     */
    float setRelativeThickness(float relativeThickness);
    void setRelativeHeight(float relativeHeight);
    //will be clipped so that walls don't leave tilebounds
    QVector2D setRelativePosition(QVector2D relativePosition);

    /**
     * Sets all relative values to zero,
     * and changes the tileTemplate to newTileTemplate
     */
    void resetTile(TileTemplate *newTileTemplate);

signals:
    void tileChanged(int x, int y);
    void tilePinged(int x, int y);

public slots:
    //by calling the respective set functions, it is ensured that the tile wont go out of bounds.
    //the signal tileChanged is also emited as expected
    void templateThicknessChanged() { setRelativePosition(mRelativePosition); }
    void templatePositionChanged() { setRelativeThickness(mRelativeThickness); }

private:
    void makeTemplateConnections();

    TileTemplate *mTileTemplate;

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

    //let w = (1 - mSize) / 2;
    //then both x and y are between -w and w (inclusive)
    //This is the offset of the tile relative to the center of it's grid position
    //(0.5,0.5 would be the centered regardless of the tiles coordinates)
    QVector2D mRelativePosition;

    //other properties to come as we develop further
};

#endif // TILE_H
