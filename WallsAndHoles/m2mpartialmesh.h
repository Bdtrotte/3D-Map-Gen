#ifndef M2MPARTIALMESH_H
#define M2MPARTIALMESH_H

#include <QVector>
#include <QVector3D>
#include <QSharedPointer>
#include <QMap>


#include "imageandsource.h"
#include "simpletexturedobject.h"

namespace M2M {


/// Class to wrap image info. This will be changed to use SharedImageAndSource when possible.
class ImageInfo {
public:
    ImageInfo() : mImage(nullptr) {}
    ImageInfo(const ImageInfo &other) = default;
    ImageInfo(SharedImageAndSource img) : mImage(img) {}

    SharedImageAndSource image() const { return mImage; }

    bool operator ==(const ImageInfo &other) const
    {
        return mImage.data() == other.mImage.data();
    }

    bool operator !=(const ImageInfo &other) const
    {
        return !(*this == other);
    }

private:
    SharedImageAndSource mImage;
};

/// Class to wrap Phong reflection info.
struct PhongInfo {
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    bool operator ==(const PhongInfo &other) const
    {
        return ambient == other.ambient
                && diffuse == other.diffuse
                && specular == other.specular
                && shininess == other.shininess;
    }

    bool operator !=(const PhongInfo &other) const
    {
        return !(*this == other);
    }
};


/**
 * @brief Represents and helps construct a quadrilateral with one texture and a Phong material.
 */
class Quad
{
public:
    /**
     * @brief Creates a quad with the given vertices. Vertices must be specified
     * in counterclockwise order.
     */
    Quad(QVector3D normal,
         ImageInfo texture,
         PhongInfo material,
         QVector3D v1, QVector2D t1,
         QVector3D v2, QVector2D t2,
         QVector3D v3, QVector2D t3,
         QVector3D v4, QVector2D t4);

    QVector3D vertex(int idx) const;
    QVector2D texCoord(int idx) const;

    QVector3D normal() const;

    ImageInfo imageInfo() const;
    PhongInfo phongInfo() const;


    /**
     * @brief makeVerticalQuad  Creates a vertical quad.
     * @param center            The center point of the quad in 3D space.
     * @param xzDirection       The normal direction of the quad -- this vector is assumed to be normalized.
     *                          A vertical quad's normal always has a Y component of 0.
     * @param width             The "width" of the quad (the other dimension of the quad, as opposed to height).
     * @param height            The "height" of the quad (size along Y axis).
     * @param texture           The texture of the quad.
     * @param material          The material of the quad.
     * @param upsideDown        If this is "false," the quad's vertices will be output in the order
     *                              4 3
     *                              1 2
     *                          when looking at the quad. Otherwise, the vertices are output in the order
     *                              2 1
     *                              4 3
     *                          Texture coordinates are always (0,0), (1,0), (1,1), (0,1).
     * @return                  The specified quad.
     */
    static Quad makeVerticalQuad(QVector3D center,
                                 QVector2D xzDirection,
                                 float width,
                                 float height,
                                 ImageInfo texture,
                                 PhongInfo material,
                                 bool upsideDown = false);


private:
    QVector3D mV1, mV2, mV3, mV4;
    QVector2D mT1, mT2, mT3, mT4;

    QVector3D mNormal;

    ImageInfo mTexture;
    PhongInfo mMaterial;
};


class Trig
{
public:
    Trig(ImageInfo texture,
         PhongInfo material,
         QVector3D v1, QVector2D t1,
         QVector3D v2, QVector2D t2,
         QVector3D v3, QVector2D t3);

    QVector<QVector3D> verts() const { return {mV1, mV2, mV3}; }
    QVector<QVector2D> textureCoords() const { return {mT1, mT2, mT3}; }

    QVector3D normal() const { return mNormal; }

    ImageInfo imageInfo() const { return mTexture; }
    PhongInfo phongInfo() const { return mMaterial; }

private:
    QVector3D mV1, mV2, mV3;
    QVector2D mT1, mT2, mT3;

    ImageInfo mTexture;
    PhongInfo mMaterial;

    QVector3D mNormal;
};


/// Struct that holds SimpleTexturedObject information.
/// This exists because SimpleTexturedObject is not made for partial construction.
class PreObject
{
public:
    /// Creates the PreObject with the given texture image.
    PreObject(ImageInfo img);

    /// Adds the quad to this object, but does not change this object's texture image.
    void addQuad(const Quad &q);

    void addTrig(const Trig &t);

    void addPreObject(const PreObject &o);

    /// Compiles this PreObject into a SimpleTexturedObject.
    QSharedPointer<SimpleTexturedObject> toObject() const;

    ImageInfo imageInfo() const { return mImage; }

private:
    // Face information.
    QVector<QVector3D> mVertexPositions;
    QVector<QVector3D> mTriangleNormals;
    QVector<SimpleTexturedObject::Triangle> mTriangles;

    // Material information.
    QVector<float> mReflAmbient;
    QVector<float> mReflDiffuse;
    QVector<float> mReflSpecular;
    QVector<float> mShininess;

    // Texture information.
    QVector<SimpleTexturedObject::TriangleTexCoords> mTriangleTextureCoordinates;
    SharedImageAndSource mImage;
};

/**
 * @brief Class to allow piece-by-piece mesh construction.
 */
class PartialMeshData
{
public:

    /**
     * @brief   Constructs a list of meshes out of the data stored in this class.
     * @return  A list of meshes.
     */
    QVector<QSharedPointer<SimpleTexturedObject>> constructObjects();

    /**
     * @brief addQuad   Adds a quad to the mesh.
     * @param q         The quad.
     */
    void addQuad(const Quad &q);

    void addTrig(const Trig &t);

    void addPartialMesh(const PartialMeshData &p);

    PartialMeshData &operator +=(const PartialMeshData &other)
    {
        addPartialMesh(other);
        return *this;
    }

    PartialMeshData &operator +(const PartialMeshData &other)
    {
        PartialMeshData p = *this;
        return p += other;
    }

private:
    /// Keeps track of one PreObject per texture image.
    QMap<const QImage *, PreObject> mTexturesToObjects;
};


}

#endif // M2MPARTIALMESH_H
