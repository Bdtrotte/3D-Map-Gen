#ifndef TILETEMPLATE_H
#define TILETEMPLATE_H

#define MIN_TILE_THICKNESS 0.001

#include <QSharedPointer>
#include <QObject>
#include <QVector2D>
#include <QColor>

class TileTemplate : public QObject
{
    Q_OBJECT

public:
    explicit TileTemplate(QColor color = Qt::white,
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

    QString name() const { return mName; }
    void setName(QString name) { mName = name; emit changed(); }

    float height() const { return mHeight; }
    float thickness() const { return mThickness; }
    QVector2D position() const { return mPosition; }

    QColor color() const { return mColor; }

    /**
     * @brief emitTilePing
     * Sends a signal to all tiles using this template, which will
     * be forwarded to the containing TileMap, and handled there.
     * Useful for gathing all tiles used by this template.
     */
    void emitTilePing() { emit pingTiles(); }

signals:
    //a property which has no affect on other properties (ie not thickness or position)
    //but rendering should be updated
    void exclusivePropertyChanged();
    void thicknessChanged();
    void positionChanged();

    //emited anytime anything which needs to be saved changes
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
};

#endif // TILETEMPLATE_H
