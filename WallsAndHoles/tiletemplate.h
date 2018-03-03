#ifndef TILETEMPLATE_H
#define TILETEMPLATE_H

#define MIN_TILE_THICKNESS 0.1

#include <QSharedPointer>
#include <QObject>
#include <QVector2D>
#include <QColor>

#include "tilematerial.h"

class TileTemplate : public QObject
{
    Q_OBJECT

public:
    explicit TileTemplate(QColor color = Qt::white,
                          QString name = "New Tile Template",
                          float height = 0,
                          float thickness = 1,
                          TileMaterial *material = nullptr,
                          QVector2D position = QVector2D(0.5, 0.5),
                          QObject *parent = nullptr);


    void setHeight(float height);

    // TODO : Might be nice to pop up a message when the user tries to change the thickness or height,
    //        but it doesn't change because of the restraintes. The message would explain why, and how
    //        to make it change, and have a checkbox to not show up again?
    //        Maybe this could be implemented in a tutorial system?

    /**
     * @brief setThickness
     *
     * Tries to set the thickness of this template.
     * If increasing thickness, it is possible that it would make the bounds of the tile go outside
     * the grid bounds (for example if the position is not centered). If this is the case, the thickness
     * actually set will be less than that requested. The thickness value actually set is returned.
     *
     * @param thickness
     * @return
     */
    float setThickness(float thickness);

    /**
     * @brief setPosition
     *
     * Tries to set the position of this template.
     * If moving position away from the center, it is possible that it would make the bounds of the tile go outside
     * the grid bounds. If this is the case, the position actually set will be different
     * than that requested. The position value actually set is returned.
     *
     * @param position
     * @return
     */
    QVector2D setPosition(QVector2D position);

    void setColor(QColor color);

    /**
     * @brief Updates the material for the tile template.
     * @param material  A pointer to the new material.
     */
    void setMaterial(TileMaterial *material);


    QString name() const { return mName; }
    void setName(QString name) { mName = name; emit changed(); }

    float height() const { return mHeight; }
    float thickness() const { return mThickness; }
    QVector2D position() const { return mPosition; }

    QColor color() const { return mColor; }


    const TileMaterial *material() const { return mMaterial; }
    TileMaterial *material() { return mMaterial; }

    /**
     * @brief emitTilePing
     * Sends a signal to all tiles using this template, which will
     * be forwarded to the containing TileMap, and handled there.
     * Useful for gathing all tiles used by this template.
     */
    void emitTilePing() { emit pingTiles(); }

signals:

    /**
     * @brief Emitted when a property is changed but does not affect mesh-related properties.
     */
    void exclusivePropertyChanged();
    void thicknessChanged();
    void positionChanged();
    void materialChanged();

    /**
     * @brief Emitted whenever something changes that needs to be saved.
     */
    void changed();

    void pingTiles();

private:
    QString mName;

    float mHeight;
    float mThickness;
    QVector2D mPosition;

    //The color the tile will be in the map view.
    //Has no affect on evental output mesh
    QColor mColor;


    TileMaterial *mMaterial;
};

#endif // TILETEMPLATE_H
