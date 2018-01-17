
#include <numeric>
#include <cassert>

#include "renderableobject.h"

RenderableObject::RenderableObject() {

}

RenderableObject::RenderableObject(std::vector<QVector3D> vertices)
    : mVertexPositions(vertices),
        mTriangleIndices(vertices.size(), 0) {

    assert( vertices.size() % 3 == 0 );

    // Sets mTriangleIndices to {0, 1, 2, ..., vertices.size()}
    std::iota(mTriangleIndices.begin(), mTriangleIndices.end(), 0);
}

RenderableObject::RenderableObject(std::vector<QVector3D> vertices, std::vector<unsigned int> indices)
    : mVertexPositions(vertices),
        mTriangleIndices(indices) {
    assert( indices.size() % 3 == 0 );
}
