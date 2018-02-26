#include "m2mpartialmesh.h"


using namespace M2M;


/* BEGIN Quad */
Quad::Quad(QVector3D normal, ImageInfo texture,
            PhongInfo material,
            QVector3D v1, QVector2D t1,
            QVector3D v2, QVector2D t2,
            QVector3D v3, QVector2D t3,
            QVector3D v4, QVector2D t4)
    : mV1(v1), mV2(v2), mV3(v3), mV4(v4)
    , mT1(t1), mT2(t2), mT3(t3), mT4(t4)
    , mNormal(normal)
    , mTexture(texture)
    , mMaterial(material)
{

}

QVector3D Quad::vertex(int idx) const
{
    QVector3D verts[4] = {mV1, mV2, mV3, mV4};
    return verts[idx];
}

QVector2D Quad::texCoord(int idx) const
{
    QVector2D texCoords[4] = {mT1, mT2, mT3, mT4};
    return texCoords[idx];
}

QVector3D Quad::normal() const
{
    return mNormal;
}

ImageInfo Quad::imageInfo() const
{
    return mTexture;
}

PhongInfo Quad::phongInfo() const
{
    return mMaterial;
}
/* END Quad */



/* BEGIN PartialMeshData */
void PartialMeshData::addQuad(Quad q)
{
    const QImage *quadImage = q.imageInfo().image->image().data();

    auto itr = mTexturesToObjects.find(quadImage);

    // If the map contains the key...
    if (itr != mTexturesToObjects.end())
        itr->addQuad(q);

    // If the map doesn't contain the key...
    else {
        PreObject newObject(q.imageInfo());
        newObject.addQuad(q);

        mTexturesToObjects.insert(quadImage, newObject);
    }
}


QVector<QSharedPointer<SimpleTexturedObject>> PartialMeshData::constructObjects()
{
    QVector<QSharedPointer<SimpleTexturedObject>> objects;

    for (const auto &preObject : mTexturesToObjects)
        objects.append(preObject.toObject());

    return objects;
}
/* END PartialMeshData */


/* BEGIN PartialMeshData::PreObject */
PartialMeshData::PreObject::PreObject(ImageInfo img)
    : mImage(img.image) {}


void PartialMeshData::PreObject::addQuad(Quad q)
{
    // index of first vertex of quad in mVertexPositions
    unsigned int firstIdx = mVertexPositions.size();


    // One entry per vertex.
    mVertexPositions.append({ q.vertex(0), q.vertex(1), q.vertex(2), q.vertex(3) });

    auto phongInfo = q.phongInfo();
    mReflAmbient.append(QVector<float>(4, phongInfo.ambient));
    mReflDiffuse.append(QVector<float>(4, phongInfo.diffuse));
    mReflSpecular.append(QVector<float>(4, phongInfo.specular));
    mShininess.append(QVector<float>(4, phongInfo.shininess));

    // One entry per triangle.
    mTriangleNormals.append({ q.normal(), q.normal() });
    mTriangles.append({
                          {firstIdx, firstIdx+1, firstIdx+2},
                          {firstIdx, firstIdx+2, firstIdx+3}
                      });
    mTriangleTextureCoordinates.append({
                                           { q.texCoord(0), q.texCoord(1), q.texCoord(2) },
                                           { q.texCoord(0), q.texCoord(2), q.texCoord(3) }
                                       });
}


QSharedPointer<SimpleTexturedObject> PartialMeshData::PreObject::toObject() const
{
    QSharedPointer<SimpleTexturedObject> obj = QSharedPointer<SimpleTexturedObject>::create();

    obj->setTriangleInfo(mVertexPositions, mTriangleNormals, mTriangles);
    obj->setMaterialInfo(mReflAmbient, mReflDiffuse, mReflSpecular, mShininess);
    obj->setTextureInfo(mTriangleTextureCoordinates, mImage);

    obj->commit();

    return obj;
}
/* END PartialMeshData::PreObject */
