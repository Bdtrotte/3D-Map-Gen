#include "m2mtilemesher.h"
#include "map2mesh.h"

QSharedPointer<SimpleTexturedObject> M2MTileMesher::getTopMesh(Input tileProperties, QVector3D offset, float scale) {

    float baseHeight = tileProperties.baseHeight;
    float topHeight = tileProperties.topHeight;

    // The height at which the sides should switch to the "dirt-to-grass" texture.
    float switchHeight = topHeight - 1;


    QVector<QVector3D> topVertices = {
        // Top (normal (0, 1, 0))
        offset + scale * QVector3D(-.5, topHeight, -.5),
        offset + scale * QVector3D(-.5, topHeight, .5),
        offset + scale * QVector3D(.5, topHeight, .5),
        offset + scale * QVector3D(.5, topHeight, -.5)
    };


    SimpleTexturedObject::TriangleTexCoords grass1 = { {0.5, 0.5}, {1, 0.5}, {1, 1} };
    SimpleTexturedObject::TriangleTexCoords grass2 = { {0.5, 0.5}, {1, 1}, {0.5, 1} };



    QVector<SimpleTexturedObject::TriangleTexCoords> texCoords = {
        // Top face.
        grass1, grass2
    };


    QVector<QVector3D> vertices;
    vertices.append(topVertices);

    QVector<QVector3D> faceNormals = {
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0)
    };


    QVector<SimpleTexturedObject::Triangle> triangles = {
        {0, 1, 2},
        {0, 2, 3}
    };



    float sideTopBase = std::max(baseHeight, switchHeight);

    if (topHeight - baseHeight > 0.01) {
        QVector<QVector3D> sideTops = {
            // Side 1 (normal (-1, 0, 0))
            offset + scale * QVector3D(-.5, sideTopBase, -.5),
            offset + scale * QVector3D(-.5, sideTopBase, .5),
            offset + scale * QVector3D(-.5, topHeight, .5),
            offset + scale * QVector3D(-.5, topHeight, -.5),


            // Side 2 (normal (1, 0, 0))
            offset + scale * QVector3D(.5, sideTopBase, .5),
            offset + scale * QVector3D(.5, sideTopBase, -.5),
            offset + scale * QVector3D(.5, topHeight, -.5),
            offset + scale * QVector3D(.5, topHeight, .5),


            // Side 3 (normal (0, 0, -1))
            offset + scale * QVector3D(.5, sideTopBase, -.5),
            offset + scale * QVector3D(-.5, sideTopBase, -.5),
            offset + scale * QVector3D(-.5, topHeight, -.5),
            offset + scale * QVector3D(.5, topHeight, -.5),


            // Side 4 (normal (0, 0, 1))
            offset + scale * QVector3D(-.5, sideTopBase, .5),
            offset + scale * QVector3D(.5, sideTopBase, .5),
            offset + scale * QVector3D(.5, topHeight, .5),
            offset + scale * QVector3D(-.5, topHeight, .5)
        };

        QVector<QVector3D> sideTopNormals = {
            QVector3D(-1, 0, 0),
            QVector3D(-1, 0, 0),
            QVector3D(1, 0, 0),
            QVector3D(1, 0, 0),
            QVector3D(0, 0, -1),
            QVector3D(0, 0, -1),
            QVector3D(0, 0, 1),
            QVector3D(0, 0, 1)
        };

        QVector<SimpleTexturedObject::Triangle> sideTopTriangles = {
            {4, 5, 6},
            {4, 6, 7},

            {8, 9, 10},
            {8, 10, 11},

            {12, 13, 14},
            {12, 14, 15},

            {16, 17, 18},
            {16, 18, 19}
        };


        // "bottom" of the dirt-grass square is adjusted to match sizes of tops.
        SimpleTexturedObject::TriangleTexCoords dirtGrass1 = { {0.5, 0.5f * (float)(1 - (topHeight - sideTopBase))}, {1, 0.5f * (float)(1 - (topHeight - sideTopBase))}, {1, 0.5} };
        SimpleTexturedObject::TriangleTexCoords dirtGrass2 = { {0.5, 0.5f * (float)(1 - (topHeight - sideTopBase))}, {1, 0.5}, {0.5, 0.5} };
        QVector<SimpleTexturedObject::TriangleTexCoords> sideTopTexCoords = {
            // Sides.
            dirtGrass1, dirtGrass2,
            dirtGrass1, dirtGrass2,
            dirtGrass1, dirtGrass2,
            dirtGrass1, dirtGrass2
        };


        vertices.append(sideTops);
        triangles.append(sideTopTriangles);
        faceNormals.append(sideTopNormals);
        texCoords.append(sideTopTexCoords);





        if (switchHeight > baseHeight) {
            // Need to add bottoms to sides.

            QVector<QVector3D> sideBottoms = {
                // Side 1 (normal (-1, 0, 0))
                offset + scale * QVector3D(-.5, baseHeight, -.5),
                offset + scale * QVector3D(-.5, baseHeight, .5),
                offset + scale * QVector3D(-.5, sideTopBase, .5),
                offset + scale * QVector3D(-.5, sideTopBase, -.5),


                // Side 2 (normal (1, 0, 0))
                offset + scale * QVector3D(.5, baseHeight, .5),
                offset + scale * QVector3D(.5, baseHeight, -.5),
                offset + scale * QVector3D(.5, sideTopBase, -.5),
                offset + scale * QVector3D(.5, sideTopBase, .5),


                // Side 3 (normal (0, 0, -1))
                offset + scale * QVector3D(.5, baseHeight, -.5),
                offset + scale * QVector3D(-.5, baseHeight, -.5),
                offset + scale * QVector3D(-.5, sideTopBase, -.5),
                offset + scale * QVector3D(.5, sideTopBase, -.5),


                // Side 4 (normal (0, 0, 1))
                offset + scale * QVector3D(-.5, baseHeight, .5),
                offset + scale * QVector3D(.5, baseHeight, .5),
                offset + scale * QVector3D(.5, sideTopBase, .5),
                offset + scale * QVector3D(-.5, sideTopBase, .5)
            };

            QVector<QVector3D> sideBottomNormals = {
                QVector3D(-1, 0, 0),
                QVector3D(-1, 0, 0),
                QVector3D(1, 0, 0),
                QVector3D(1, 0, 0),
                QVector3D(0, 0, -1),
                QVector3D(0, 0, -1),
                QVector3D(0, 0, 1),
                QVector3D(0, 0, 1)
            };



            float h = sideTopBase - baseHeight;
            float ty = 1 - .5 * h;
            SimpleTexturedObject::TriangleTexCoords dirt1 = { {0, ty}, {0.5, ty}, {0.5, 1} };
            SimpleTexturedObject::TriangleTexCoords dirt2 = { {0, ty}, {0.5, 1}, {0, 1} };
            QVector<SimpleTexturedObject::TriangleTexCoords> bottomTexCoords = {
                dirt1, dirt2,
                dirt1, dirt2,
                dirt1, dirt2,
                dirt1, dirt2
            };


            QVector<SimpleTexturedObject::Triangle> bottomTriangles = {
                {20, 21, 22},
                {20, 22, 23},

                {24, 25, 26},
                {24, 26, 27},

                {28, 29, 30},
                {28, 30, 31},

                {32, 33, 34},
                {32, 34, 35}
            };


            vertices.append(sideBottoms);
            faceNormals.append(sideBottomNormals);
            texCoords.append(bottomTexCoords);
            triangles.append(bottomTriangles);
        }
    }



    QSharedPointer<QImage> textureImage = tileProperties.image.isNull() ? getTestingImage() : tileProperties.image;

    QSharedPointer<SimpleTexturedObject> obj = QSharedPointer<SimpleTexturedObject>::create();

    obj->setTriangleInfo(vertices, faceNormals, triangles);
    obj->setMaterialInfo(
                tileProperties.ambient,
                tileProperties.diffuse,
                tileProperties.specular,
                tileProperties.shininess);
    obj->setTextureInfo(texCoords, textureImage);
    obj->commit();

    return obj;
}


QSharedPointer<QImage> M2MTileMesher::TestingImage = nullptr;
QSharedPointer<QImage> M2MTileMesher::getTestingImage()
{
    if (TestingImage.isNull())
        TestingImage = QSharedPointer<QImage>::create("://images/textures/exampleTexture.png");

    return TestingImage;
}
