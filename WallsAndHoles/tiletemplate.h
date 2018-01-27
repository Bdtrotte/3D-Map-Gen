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
    explicit TileTemplate(float height = 0,
                          float thickness = 0,
                          QVector2D position = QVector2D(0.5, 0.5),
                          QColor color = Qt::white,
                          QObject *parent = nullptr);

    void setHeight(float height);

    // TODO: these should ensure the resulting tile stays inbounds
    void setThickness(float thickness);
    void setPosition(QVector2D position);

    void setColor(QColor color);

    float height() const { return mHeight; }
    float thickness() const { return mThickness; }
    QVector2D position() const { return mPosition; }

    QColor color() const { return mColor; }

signals:
    //a property which has no affect on other properties (ie not thickness or position)
    //but rendering should be updated
    void exclusivePropertyChanged();
    void thicknessChanged();
    void positionChanged();

private:
    float mHeight;
    float mThickness;
    QVector2D mPosition;

    //The color the tile will be in the map view.
    //Has no affect on evental output mesh
    QColor mColor;
};

typedef QSharedPointer<TileTemplate> SharedTileTemplate;

#endif // TILETEMPLATE_H
