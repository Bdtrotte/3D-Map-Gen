#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include "meshmaterial.h"


/**
 * @brief The RenderableObject class
 *
 * Stores the most basic information needed to render an object (just the vertex positions).
 */
class RenderableObject {
public:

    RenderableObject();
    RenderableObject(QVector<QVector3D> vertices);
    RenderableObject(QVector<QVector3D> vertices, QVector<unsigned int> indices);
    RenderableObject(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<unsigned int> indices);
    RenderableObject(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<MeshMaterial> materials, QVector<unsigned int> indices);

    const QVector<QVector3D>& getVertexData() const { return mVertexPositions; }    
    const QVector<QVector3D>& getVertexNormals() const { return mVertexNormals; }
    const QVector<MeshMaterial>& getVertexMaterials() const { return mVertexMaterials; }

    const QVector<unsigned int>& getTriangleIndices() const { return mTriangleIndices; }

    size_t getNumVertices() const { return mVertexPositions.size(); }

protected:

    /**
     * @brief Generate normal vectors automatically, assuming mVertexPositions
     * and mTriangleIndices are set. This assumes that triangle indices are
     * specified in counterclockwise order, that is if the triangle is rendered
     * facing the camera, then the indices enumerate the vertices counterclockwise.
     */
    void generateNormals();

    QVector<QVector3D> mVertexPositions;
    QVector<QVector3D> mVertexNormals;
    QVector<MeshMaterial> mVertexMaterials;

    /**
     * @brief mTriangleIndices Stores a list of indices to the mVertex_ arrays above.
     * Every three indices correspond to a triangle.
     */
    QVector<unsigned int> mTriangleIndices;
};


#endif // RENDERABLEOBJECT_H
