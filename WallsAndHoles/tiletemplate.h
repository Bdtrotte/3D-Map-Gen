#ifndef TILETEMPLATE_H
#define TILETEMPLATE_H

#define MIN_TILE_THICKNESS 0.001

#include <QSharedPointer>
#include <QObject>
#include <QVector2D>
#include <QColor>

// For std::unique_ptr
#include <memory>

#include "simpletexturedmaterial.h"

class TileTemplate : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Creates a TileTemplate with the default material.
     */
    explicit TileTemplate(QColor color = Qt::white,
                          QString name = "New Tile Template",
                          float height = 0,
                          float thickness = 1,
                          QVector2D position = QVector2D(0.5, 0.5),
                          QObject *parent = nullptr);


    /**
     * @brief Creates a TileMaterial with a particular material (which must be
     * at least a SimpleTexturedMaterial).
     *
     * Note: material should either be an rvalue or passed with std::move()
     */
    explicit TileTemplate(std::unique_ptr<SimpleTexturedMaterial> &&material,
                          QColor color = Qt::white,
                          QString name = "New Tile Template",
                          float height = 0,
                          float thickness = 1,
                          QVector2D position = QVector2D(0.5, 0.5),
                          QObject *parent = nullptr);

    void setHeight(float height);

    // TODO: these should ensure the resulting tile stays inbounds
    void setThickness(float thickness);
    void setPosition(QVector2D position);

    void setColor(QColor color);

    /**
     * @brief Updates the material for the tile template.
     * @param material  A pointer to the new material.
     */
    void setMaterial(std::unique_ptr<SimpleTexturedMaterial> &&material);


    QString name() const { return mName; }
    void setName(QString name) { mName = name; emit changed(); }

    float height() const { return mHeight; }
    float thickness() const { return mThickness; }
    QVector2D position() const { return mPosition; }

    QColor color() const { return mColor; }


    // Implementation note: return-by-reference used to avoid slicing.
    const SimpleTexturedMaterial &material() const { return *mMaterial; }

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

    /**
     * @brief The material this tile template uses.
     *
     * For now, all tile templates will use SimpleTexturedMaterials. Later on,
     * when we choose to add in cooler rendering, SimpleTexturedMaterial
     * may get extended.
     *
     * This is a pointer to allow for polymorphism.
     * This is a smart pointer to avoid memory leaks and to make ownership explicit.
     * This is a unique pointer because the material should only be editable from within this class.
     *
     * Implementation note: TileTemplate should never construct a SimpleTexturedMaterial
     * because it should not assume what subclass of the material is being used.
     */
    std::unique_ptr<SimpleTexturedMaterial> mMaterial;



    static QSharedPointer<QImage> getDefaultTexture();
    static QSharedPointer<QImage> DefaultTexture;
};

#endif // TILETEMPLATE_H
