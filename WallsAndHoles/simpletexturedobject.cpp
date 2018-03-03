#include "simpletexturedobject.h"

SimpleTexturedObject::SimpleTexturedObject()
    : mCommitted(false)
{

}

SimpleTexturedObject::~SimpleTexturedObject()
{

}


void SimpleTexturedObject::commit()
{
    // All of these arrays must be parallel to the vertex positions array.
    Q_ASSERT(getNumVertices() == mReflAmbient.size());
    Q_ASSERT(getNumVertices() == mReflDiffuse.size());
    Q_ASSERT(getNumVertices() == mReflSpecular.size());
    Q_ASSERT(getNumVertices() == mShininess.size());

    // All of these arrays must be parallel to the triangles array.
    Q_ASSERT(getNumTriangles() == mTriangleNormals.size());
    Q_ASSERT(getNumTriangles() == mTriangleTextureCoordinates.size());


    mCommitted = true;

    emit objectUpdated();
}



void SimpleTexturedObject::setTriangleInfo(QVector<QVector3D> positions, QVector<QVector3D> normals, QVector<Triangle> triangles)
{
    // The normals should be specified per-face.
    Q_ASSERT(normals.size() == triangles.size());

    mVertexPositions = positions;
    mTriangleNormals = normals;
    mTriangles = triangles;

    mCommitted = false;
}

void SimpleTexturedObject::setMaterialInfo(float ambient, float diffuse, float specular, float shininess)
{
    mReflAmbient = QVector<float>(getNumVertices(), ambient);
    mReflDiffuse = QVector<float>(getNumVertices(), diffuse);
    mReflSpecular = QVector<float>(getNumVertices(), specular);
    mShininess = QVector<float>(getNumVertices(), shininess);

    mCommitted = false;
}


void SimpleTexturedObject::setMaterialInfo(QVector<float> ambient, QVector<float> diffuse, QVector<float> specular, QVector<float> shininess)
{
    // It is assumed that setTriangleInfo() has been called and that the
    // arrays are parallel to the vertex position array.
    Q_ASSERT(ambient.size() == getNumVertices());
    Q_ASSERT(diffuse.size() == getNumVertices());
    Q_ASSERT(specular.size() == getNumVertices());
    Q_ASSERT(shininess.size() == getNumVertices());

    mReflAmbient = ambient;
    mReflDiffuse = diffuse;
    mReflSpecular = specular;
    mShininess = shininess;

    mCommitted = false;
}


void SimpleTexturedObject::setTextureInfo(QVector<TriangleTexCoords> texCoords, SharedImageAndSource image)
{
    // It is assumed that setTriangleInfo() has been called and that the
    // texCoords array is parallel to the triangles array.
    Q_ASSERT(texCoords.size() == getNumTriangles());

    mTriangleTextureCoordinates = texCoords;
    mImage = image;

    mCommitted = false;
}


bool SimpleTexturedObject::isCommitted() const
{
    return mCommitted;
}


int SimpleTexturedObject::getNumVertices() const
{
    return mVertexPositions.size();
}


int SimpleTexturedObject::getNumTriangles() const
{
    return mTriangles.size();
}

const QVector<QVector3D> &SimpleTexturedObject::getVertices() const
{
    Q_ASSERT(isCommitted());
    return mVertexPositions;
}

const QVector<QVector3D> &SimpleTexturedObject::getFaceNormals() const
{
    Q_ASSERT(isCommitted());
    return mTriangleNormals;
}

const QVector<SimpleTexturedObject::Triangle> &SimpleTexturedObject::getTriangles() const
{
    Q_ASSERT(isCommitted());
    return mTriangles;
}

const QVector<float> &SimpleTexturedObject::getVertexAmbient() const
{
    Q_ASSERT(isCommitted());
    return mReflAmbient;
}

const QVector<float> &SimpleTexturedObject::getVertexDiffuse() const
{
    Q_ASSERT(isCommitted());
    return mReflDiffuse;
}

const QVector<float> &SimpleTexturedObject::getVertexSpecular() const
{
    Q_ASSERT(isCommitted());
    return mReflSpecular;
}

const QVector<float> &SimpleTexturedObject::getVertexShininess() const
{
    Q_ASSERT(isCommitted());
    return mShininess;
}

const QVector<SimpleTexturedObject::TriangleTexCoords> &SimpleTexturedObject::getFaceTexCoords() const
{
    Q_ASSERT(isCommitted());
    return mTriangleTextureCoordinates;
}

const QImage &SimpleTexturedObject::getImage() const
{
    Q_ASSERT(isCommitted());
    Q_ASSERT(!mImage.isNull());
    return *mImage->image();
}

SharedImageAndSource SimpleTexturedObject::getImageAndSource() const
{
    return mImage;
}
