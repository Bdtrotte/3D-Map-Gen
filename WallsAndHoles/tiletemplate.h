#ifndef TILETEMPLATE_H
#define TILETEMPLATE_H

#include <QSharedPointer>
#include <QObject>

class TileTemplate : public QObject
{
    Q_OBJECT

public:
    explicit TileTemplate(float height = 0, float thickness = 0, QObject *parent = nullptr);

    void setTileId(int id) { mTileId = id; }
    void setHeight(float height);
    void setThickness(float thickness);

    const int tileId() { return mTileId; }
    const float height() { return mHeight; }
    const float thickness() { return mThickness; }

signals:
    void propertyChanged(int tileId);

private:
    int mTileId;
    float mHeight;
    float mThickness;
};

typedef QSharedPointer<TileTemplate> SharedTileTemplate;

#endif // TILETEMPLATE_H
