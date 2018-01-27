
#include <numeric>
#include <cassert>

#include "renderableobject.h"

RenderableObject::RenderableObject() {

}

RenderableObject::RenderableObject(QVector<QVector3D> vertices)
    : mVertexPositions(vertices),
      mVertexMaterials(vertices.size(), MeshMaterial(1, 1, 1, 1)),
      mTriangleIndices(vertices.size(), 0) {

    Q_ASSERT( vertices.size() % 3 == 0 );

    // Sets mTriangleIndices to {0, 1, 2, ..., vertices.size()}
    std::iota(mTriangleIndices.begin(), mTriangleIndices.end(), 0);

    generateNormals();
}

RenderableObject::RenderableObject(QVector<QVector3D> vertices, QVector<unsigned int> indices)
    : mVertexPositions(vertices),
      mVertexMaterials(vertices.size(), MeshMaterial(1, 1, 1, 1)),
      mTriangleIndices(indices) {

    Q_ASSERT( indices.size() % 3 == 0 );

    generateNormals();
}


RenderableObject::RenderableObject(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<unsigned int> indices)
    : mVertexPositions(vertices),
      mVertexNormals(normals),
      mVertexMaterials(vertices.size(), MeshMaterial(1, 1, 1, 1)),
      mTriangleIndices(indices) {

    Q_ASSERT( indices.size() % 3 == 0 );
}

RenderableObject::RenderableObject(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<MeshMaterial> materials, QVector<unsigned int> indices)
    : mVertexPositions(vertices),
      mVertexNormals(normals),
      mVertexMaterials(materials),
      mTriangleIndices(indices) {

    Q_ASSERT( indices.size() % 3 == 0 );
}



void RenderableObject::generateNormals() {
    // The normal of a vertex will be the average of the geometric normal vectors of the triangles incident on it.

    mVertexNormals = QVector<QVector3D>(mVertexPositions.size(), QVector3D(0, 0, 0));

    // 1) Accumulate.
    for (int i = 0; i < mTriangleIndices.size(); i += 3) {
        QVector3D v1 = mVertexPositions[i*3 + 0],
                v2 = mVertexPositions[i*3 + 1],
                v3 = mVertexPositions[i*3 + 2];

        QVector3D diff1 = v2 - v1,
                diff2 = v3 - v1;

        QVector3D normal = QVector3D::normal(diff1, diff2);

        mVertexNormals[i*3 + 0] += normal;
        mVertexNormals[i*3 + 1] += normal;
        mVertexNormals[i*3 + 2] += normal;
    }

    // 2) Normalize.
    for (int i = 0; i < mVertexNormals.size(); ++i) {
        QVector3D normal = mVertexNormals[i];

        // If this assert fails, that means that the sum of normal vectors for a vertex
        // is 0. This is almost certainly because triangle indices are not specified
        // in a consistent order -- see the comment at the declaration of generateNormals()
        // in renderableobject.h
        Q_ASSERT( normal.lengthSquared() != 0 );

        mVertexNormals[i] = normal.normalized();
    }
}
