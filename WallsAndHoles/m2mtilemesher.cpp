#include "m2mtilemesher.h"
#include "map2mesh.h"

#include "meshmaterial.h"

QSharedPointer<SimpleTexturedObject> M2MTileMesher::getTopMesh(const M2MPropertySet &tileProperties, QVector3D offset, float scale) {

    M2MPropertyInstance propHeight = tileProperties.getProperty(Map2Mesh::Properties::Height);

    float baseHeight = propHeight.getParameter(Map2Mesh::Properties::Height, "baseHeight");
    float topHeight = propHeight.getParameter(Map2Mesh::Properties::Height, "topHeight");


    QVector<QVector3D> vertices = {
        // Top (normal (0, 1, 0))
        offset + scale * QVector3D(-.5, topHeight, -.5),
        offset + scale * QVector3D(-.5, topHeight, .5),
        offset + scale * QVector3D(.5, topHeight, .5),
        offset + scale * QVector3D(.5, topHeight, -.5),


        // Side 1 (normal (-1, 0, 0))
        offset + scale * QVector3D(-.5, topHeight, -.5),
        offset + scale * QVector3D(-.5, baseHeight, -.5),
        offset + scale * QVector3D(-.5, baseHeight, .5),
        offset + scale * QVector3D(-.5, topHeight, .5),


        // Side 2 (normal (1, 0, 0))
        offset + scale * QVector3D(.5, topHeight, -.5),
        offset + scale * QVector3D(.5, topHeight, .5),
        offset + scale * QVector3D(.5, baseHeight, .5),
        offset + scale * QVector3D(.5, baseHeight, -.5),


        // Side 3 (normal (0, 0, -1))
        offset + scale * QVector3D(-.5, topHeight, -.5),
        offset + scale * QVector3D(.5, topHeight, -.5),
        offset + scale * QVector3D(.5, baseHeight, -.5),
        offset + scale * QVector3D(-.5, baseHeight, -.5),


        // Side 4 (normal (0, 0, 1))
        offset + scale * QVector3D(-.5, topHeight, .5),
        offset + scale * QVector3D(-.5, baseHeight, .5),
        offset + scale * QVector3D(.5, baseHeight, .5),
        offset + scale * QVector3D(.5, topHeight, .5)
    };

    QVector<QVector3D> faceNormals = {
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0),
        QVector3D(-1, 0, 0),
        QVector3D(-1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, 1),
        QVector3D(0, 0, 1)
    };

    QVector<SimpleTexturedObject::Triangle> triangles = {
        {0, 1, 2},
        {0, 2, 3},

        {4, 5, 6},
        {4, 6, 7},

        {8, 9, 10},
        {8, 10, 11},

        {12, 13, 14},
        {12, 14, 15},

        {16, 17, 18},
        {16, 18, 19}
    };

    QVector<SimpleTexturedObject::TriangleTexCoords> texCoords
            = QVector<SimpleTexturedObject::TriangleTexCoords>(triangles.size(), {{0,0},{0,0},{0,0}});

    QSharedPointer<SimpleTexturedObject> obj = QSharedPointer<SimpleTexturedObject>::create();

    MeshMaterial mat = MeshMaterial::Soft;

    obj->setTriangleInfo(vertices, faceNormals, triangles);
    obj->setMaterialInfo(mat.getReflAmbient(), mat.getReflDiffuse(), mat.getReflSpecular(), mat.getShininess());
    obj->setTextureInfo(texCoords, nullptr);
    obj->commit();

    return obj;
}
