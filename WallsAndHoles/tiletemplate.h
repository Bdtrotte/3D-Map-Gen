#ifndef TILETEMPLATE_H
#define TILETEMPLATE_H

#define MIN_TILE_THICKNESS 0.001

#include <QSharedPointer>
#include <QObject>
#include <QVector2D>

class TileTemplate : public QObject
{
    Q_OBJECT

public:
    explicit TileTemplate(float height = 0,
                          float thickness = 0,
                          QVector2D position = QVector2D(0.5, 0.5),
                          QObject *parent = nullptr);

    void setTileId(int id) { mTileId = id; }
    void setHeight(float height);
    void setThickness(float thickness);
    void setPosition(QVector2D position);

    int tileId() const { return mTileId; }
    float height() const { return mHeight; }
    float thickness() const { return mThickness; }
    QVector2D position() const { return mPosition; }

signals:
    void propertyChanged(int tileId);

private:
    int mTileId;
    float mHeight;
    float mThickness;
    QVector2D mPosition;
};

typedef QSharedPointer<TileTemplate> SharedTileTemplate;

#endif // TILETEMPLATE_H
