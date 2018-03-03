#ifndef SIMPLETEXTUREDOBJECT_H
#define SIMPLETEXTUREDOBJECT_H


#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include <QImage>

#include "imageandsource.h"
#include "triplet.h"

/**
 * @brief The SimpleTexturedObject class represents an object with Phong lighting information
 * and a single texture.
 */
class SimpleTexturedObject : public QObject
{
    Q_OBJECT

public:

    using Triangle = Triplet<unsigned int, unsigned int, unsigned int>;
    using TriangleTexCoords = Triplet<QVector2D, QVector2D, QVector2D>;


    /**
     * @brief Creates an empty object. Use the setter methods below to set up
     * data, and then call commit() when done.
     */
    SimpleTexturedObject();

    virtual ~SimpleTexturedObject();

    /**
     * @brief Call this method after setting all of the object's information.
     * This will commit the information and allow it to be rendered.
     */
    void commit();


    /**
     * @brief Sets up face information for the object.
     * @param positions     A list of vertex positions.
     * @param normals       A list of face normals. Should be parallel to triangles array.
     * @param triangles     A list of triangles. Each triangle is a triplet of indices
     *                      into the positions array.
     */
    void setTriangleInfo(
            QVector<QVector3D> positions,
            QVector<QVector3D> normals,
            QVector<Triangle> triangles);

    /**
     * @brief Sets up material information for the object. Uses the same material for
     * each vertex.
     *
     * Assumes setTriangleInfo() has been called.
     * @param ambient       The ambient parameter.
     * @param diffuse       The diffuse parameter.
     * @param specular      The specular parameter.
     * @param shininess     The shininess parameter.
     */
    void setMaterialInfo(float ambient, float diffuse, float specular, float shininess);

    /**
     * @brief Sets up per-vertex materials for the object.
     *
     * Assumes setTriangleInfo() has been called.
     * @param ambient       The ambient parameter.
     * @param diffuse       The diffuse parameter.
     * @param specular      The specular parameter.
     * @param shininess     The shininess parameter.
     */
    void setMaterialInfo(QVector<float> ambient, QVector<float> diffuse, QVector<float> specular, QVector<float> shininess);

    /**
     * @brief Sets up texture information for the object.
     *
     * Assumes setTriangleInfo() has been called.
     * @param texCoords     Texture coordinates for each triangle.
     * @param image         The image (texture) for the object.
     */
    void setTextureInfo(QVector<TriangleTexCoords> texCoords, SharedImageAndSource image);


    bool isCommitted() const;

    /**
     * @brief Returns the number of vertices in the vertex array. This may not be the same
     * as the number of vertices when the object is rendered (it may be that the triangle
     * array is unrolled).
     * @return Equal to getVertices().size() if isCommitted() is true. In any case, equal
     * to the size of the positions array last passed to setTriangleInfo().
     */
    int getNumVertices() const;

    int getNumTriangles() const;

    /* All of the below methods assert that isCommitted() is true. */
    const QVector<QVector3D> &getVertices() const;
    const QVector<QVector3D> &getFaceNormals() const;
    const QVector<Triangle> &getTriangles() const;

    const QVector<float> &getVertexAmbient() const;
    const QVector<float> &getVertexDiffuse() const;
    const QVector<float> &getVertexSpecular() const;
    const QVector<float> &getVertexShininess() const;

    const QVector<TriangleTexCoords> &getFaceTexCoords() const;
    const QImage &getImage() const;
    SharedImageAndSource getImageAndSource() const;



signals:

    /**
     * @brief Emitted when the object is updated. It is assumed that isCommitted() is true.
     */
    void objectUpdated();

private:


    // Face information.
    QVector<QVector3D> mVertexPositions;
    QVector<QVector3D> mTriangleNormals;                    /// This array is parallel to mTriangles.
    QVector<Triangle> mTriangles;

    // Material information.
    QVector<float> mReflAmbient;
    QVector<float> mReflDiffuse;
    QVector<float> mReflSpecular;
    QVector<float> mShininess;

    // Texture information.
    QVector<TriangleTexCoords> mTriangleTextureCoordinates; /// This array is parallel to mTriangles.
    SharedImageAndSource mImage;


    /// True if the getter methods may be used.
    bool mCommitted;
};

#endif // SIMPLETEXTUREDOBJECT_H
