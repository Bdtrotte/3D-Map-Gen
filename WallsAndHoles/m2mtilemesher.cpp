#include "m2mtilemesher.h"



RenderableObject M2MTileMesher::getTopMesh(const Tile &tile, QVector3D offset, float scale) {

    float height = tile.relativeHeight();

    if (tile.hasTileTemplate())
        height += tile.tileTemplate()->height();

    QVector<QVector3D> vertices = {
        offset + scale * QVector3D(-.5, height, -.5),
        offset + scale * QVector3D(-.5, height, .5),
        offset + scale * QVector3D(.5, height, .5),
        offset + scale * QVector3D(.5, height, -.5),
    };

    QVector<QVector3D> normals(4, QVector3D(0, 1, 0));

    QVector<unsigned int> triangles = {
        0, 1, 2,
        0, 2, 3
    };


    return RenderableObject(vertices, normals, triangles);
}
