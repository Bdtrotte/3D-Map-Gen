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


    /// @brief Constructs an object with the vertices, assuming every 3 vertices define a triangle.
    RenderableObject(QVector<QVector3D> vertices);

    /// @brief Constructs an object with the given vertices and triangle indices. Normals and materials are generated.
    RenderableObject(QVector<QVector3D> vertices, QVector<unsigned int> indices);

    /// @brief Constructs an object with the given vertices, normals & triangle indices. Note that each index selects BOTH THE
    /// VERTEX AND THE NORMAL. That is, you cannot specify separate indices for normals and vertices. A default material is used.
    RenderableObject(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<unsigned int> indices);

    /// @brief Constructs an object with the given vertices, normals, materials & triangle indices. Note that each index selects THE
    /// VERTEX, THE NORMAL AND THE MATERIAL. That is, you cannot specify separate indices for normals/vertices/materials.
    RenderableObject(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<MeshMaterial> materials, QVector<unsigned int> indices);


    const QVector<QVector3D>& getVertexData() const { return mVertexPositions; }    
    const QVector<QVector3D>& getVertexNormals() const { return mVertexNormals; }
    const QVector<MeshMaterial>& getVertexMaterials() const { return mVertexMaterials; }

    const QVector<unsigned int>& getTriangleIndices() const { return mTriangleIndices; }

    size_t getNumVertices() const { return mVertexPositions.size(); }

protected:

    /**
     * @brief Generate normal vectors automatically, assuming mVertexPositions
     * and mTriangleIndices are set.
     *
     * This assumes that triangle indices are specified in counterclockwise order,
     * that is if the triangle is rendered facing the camera, then the indices enumerate
     * the vertices counterclockwise. Currently, this is not a great option!
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
