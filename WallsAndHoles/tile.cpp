#include "tile.h"

#include <QDebug>

Tile::Tile(TileTemplate *tileTemplate,
               int xPos,
               int yPos,
               QObject *parent)
    : QObject(parent)
    , mTileTemplate(tileTemplate)
    , mXPos(xPos)
    , mYPos(yPos)
    , mRelativeThickness(0)
    , mRelativeHeight(0)
    , mRelativePosition(QVector2D())
{
    makeTemplateConnections();
}

float Tile::thickness() const
{
    if (mTileTemplate == nullptr)
        return mRelativeThickness + 1;
    else
        return mRelativeThickness + mTileTemplate->thickness();
}

float Tile::height() const
{
    if (mTileTemplate == nullptr)
        return mRelativeHeight;
    else
        return mRelativeHeight + mTileTemplate->height();
}

QVector2D Tile::position() const
{
    if (mTileTemplate == nullptr)
        return mRelativePosition + QVector2D(0.5, 0.5);
    else
        return mRelativePosition + mTileTemplate->position();
}

const TileMaterial *Tile::topMaterial() const
{
    if (mTileTemplate)
        return mTileTemplate->topMaterial();
    else
        return TileMaterial::getDefaultGroundMaterial();
}

const TileMaterial *Tile::sideMaterial() const
{
    if (mTileTemplate) {
        if (mTileTemplate->hasSideMaterial())
            return mTileTemplate->sideMaterial();
        else
            return mTileTemplate->topMaterial();
    } else {
        return TileMaterial::getDefaultGroundMaterial();
    }
}

float Tile::setRelativeThickness(float relativeThickness)
{
    if (relativeThickness == mRelativeThickness) {
        emit tileChanged(mXPos, mYPos);
        return mRelativeThickness;
    }

    if (mTileTemplate == nullptr) {
        mRelativeThickness = 0;
        emit tileChanged(mXPos, mYPos);
        return 0;
    }

    if (relativeThickness + mTileTemplate->thickness() > 1)
        relativeThickness = 1 - mTileTemplate->thickness();
    else if (relativeThickness + mTileTemplate->thickness() < MIN_TILE_THICKNESS)
        relativeThickness = -mTileTemplate->thickness() + MIN_TILE_THICKNESS;

    QVector2D pos = mRelativePosition + mTileTemplate->position();
    float thickness = relativeThickness + mTileTemplate->thickness();

    if (thickness/2 + pos.x() > 1)
        relativeThickness = 2 - 2 * pos.x() - mTileTemplate->thickness();
    else if (-thickness/2 + pos.x() < 0)
        relativeThickness = 2 * pos.x() - mTileTemplate->thickness();

    thickness = relativeThickness + mTileTemplate->thickness();

    if (thickness/2 + pos.y() > 1)
        relativeThickness = 2 - 2 * pos.y() - mTileTemplate->thickness();
    else if (-thickness/2 + pos.y() < 0)
        relativeThickness = 2 * pos.y() - mTileTemplate->thickness();

    mRelativeThickness = relativeThickness;

    emit tileChanged(mXPos, mYPos);

    return mRelativeThickness;
}

void Tile::setRelativeHeight(float relativeHeight)
{
    if (mTileTemplate == nullptr)
        relativeHeight = 0;

    mRelativeHeight = relativeHeight;

    emit tileChanged(mXPos, mYPos);
}

QVector2D Tile::setRelativePosition(QVector2D relavtivePosition)
{
    if (mTileTemplate == nullptr) {
        mRelativePosition = QVector2D();
        emit tileChanged(mXPos, mYPos);
        return QVector2D();
    }

    float thickness = mRelativeThickness + mTileTemplate->thickness();
    QVector2D pos = relavtivePosition + mTileTemplate->position();

    if (thickness/2 + pos.x() > 1)
        relavtivePosition.setX(1 - thickness/2 - mTileTemplate->position().x());
    else if (-thickness/2 + pos.x() < 0)
        relavtivePosition.setX(thickness/2 - mTileTemplate->position().x());

    if (thickness/2 + pos.y() > 1)
        relavtivePosition.setY(1 - thickness/2 - mTileTemplate->position().y());
    else if (-thickness/2 + pos.y() < 0)
        relavtivePosition.setY(thickness/2 - mTileTemplate->position().y());

    mRelativePosition = relavtivePosition;

    emit tileChanged(mXPos, mYPos);

    return mRelativePosition;
}

void Tile::resetTile(TileTemplate *newTileTemplate)
{
    mRelativeThickness = mRelativeHeight = 0;
    mRelativePosition = QVector2D();

    if (mTileTemplate != nullptr)
        mTileTemplate->disconnect(this);

    mTileTemplate = newTileTemplate;
    makeTemplateConnections();

    emit tileChanged(mXPos, mYPos);
}

void Tile::makeTemplateConnections()
{
    if (mTileTemplate != nullptr) {

        auto emitTileChanged = [this] {
            emit tileChanged(mXPos, mYPos);
        };

        auto emitTilePinged = [this] () {
            emit tilePinged(mXPos, mYPos);
        };

        connect(mTileTemplate, &TileTemplate::exclusivePropertyChanged, this, emitTileChanged);
        connect(mTileTemplate, &TileTemplate::thicknessChanged, this, &Tile::templateThicknessChanged);
        connect(mTileTemplate, &TileTemplate::positionChanged, this, &Tile::templatePositionChanged);
        connect(mTileTemplate, &TileTemplate::materialChanged, this, emitTileChanged);
        connect(mTileTemplate, &TileTemplate::pingTiles, this, emitTilePinged);
    }
}
