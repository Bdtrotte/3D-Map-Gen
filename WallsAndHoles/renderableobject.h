#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>


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

    const QVector<QVector3D>& getVertexData() const { return mVertexPositions; }
    const QVector<unsigned int>& getTriangleIndices() const { return mTriangleIndices; }

    size_t getNumVertices() const { return mVertexPositions.size(); }

protected:

    QVector<QVector3D> mVertexPositions;

    /**
     * @brief mTriangleIndices Stores a list of indices to the mVertex_ arrays above.
     * Every three indices correspond to a triangle.
     */
    QVector<unsigned int> mTriangleIndices;
};


#endif // RENDERABLEOBJECT_H
