#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

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
    RenderableObject(std::vector<QVector3D> vertices);
    RenderableObject(std::vector<QVector3D> vertices, std::vector<unsigned int> indices);

    const std::vector<QVector3D>& getVertexData() const { return mVertexPositions; }
    const std::vector<unsigned int>& getTriangleIndices() const { return mTriangleIndices; }

    size_t getNumVertices() const { return mVertexPositions.size(); }

protected:

    std::vector<QVector3D> mVertexPositions;

    /**
     * @brief mTriangleIndices Stores a list of indices to the mVertex_ arrays above.
     * Every three indices correspond to a triangle.
     */
    std::vector<unsigned int> mTriangleIndices;
};


#endif // RENDERABLEOBJECT_H
