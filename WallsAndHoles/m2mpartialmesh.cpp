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


Quad Quad::makeVerticalQuad(QVector3D center,
                            QVector2D xzDirection,
                            float width,
                            float height,
                            ImageInfo texture,
                            PhongInfo material,
                            bool upsideDown)
{
    QVector2D t1(0, 0);
    QVector2D t2(width, 0);
    QVector2D t3(width, height);
    QVector2D t4(0, height);

    if (upsideDown) xzDirection = -xzDirection;

    // "Left" direction when looking at the quad from the front.
    // Magnitude is such that center + left is on the left edge of the quad.
    QVector3D left(-xzDirection.y() * width * 0.5, 0, xzDirection.x() * width * 0.5);
    QVector3D down(0, -height*0.5, 0);

    if (upsideDown) {
        left = -left;
        down = -down;
    }

    QVector3D v1 = center + left + down;
    QVector3D v2 = center - left + down;
    QVector3D v3 = center - left - down;
    QVector3D v4 = center + left - down;

    QVector3D normal(xzDirection.x(), 0, xzDirection.y());

    return Quad(normal, texture, material,
                v1, t1,
                v2, t2,
                v3, t3,
                v4, t4);
}

/* END Quad */



/* BEGIN PartialMeshData */
void PartialMeshData::addQuad(const Quad &q)
{
    const QImage *quadImage = q.imageInfo().image()->image().data();

    auto itr = mTexturesToObjects.find(quadImage);

    // If the map contains the key...
    if (itr != mTexturesToObjects.end()) {
        itr->addQuad(q);

    // If the map doesn't contain the key...
    } else {
        PreObject newObject(q.imageInfo());
        newObject.addQuad(q);

        mTexturesToObjects.insert(quadImage, newObject);
    }
}

void PartialMeshData::addTrig(const Trig &t)
{
    const QImage *image = t.imageInfo().image()->image().data();

    auto itr = mTexturesToObjects.find(image);

    // If the map contains the key...
    if (itr != mTexturesToObjects.end()) {
        itr->addTrig(t);

    // If the map doesn't contain the key...
    } else {
        PreObject newObject(t.imageInfo());
        newObject.addTrig(t);

        mTexturesToObjects.insert(image, newObject);
    }
}

void PartialMeshData::addPartialMesh(const PartialMeshData &p)
{
    for (auto i = p.mTexturesToObjects.cbegin(); i != p.mTexturesToObjects.cend(); ++i) {
        auto itr = mTexturesToObjects.find(i.key());
        if (itr != mTexturesToObjects.end())
            itr->addPreObject(*i);
        else
            mTexturesToObjects.insert(i.key(), *i);
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
PreObject::PreObject(ImageInfo img)
    : mImage(img.image()) {}


void PreObject::addQuad(const Quad &q)
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

void PreObject::addTrig(const Trig &t)
{
    unsigned int firstIdx = mVertexPositions.size();

    mVertexPositions.append(t.verts());
    PhongInfo phongInfo = t.phongInfo();
    mReflAmbient.append(QVector<float>(3, phongInfo.ambient));
    mReflDiffuse.append(QVector<float>(3, phongInfo.diffuse));
    mReflSpecular.append(QVector<float>(3, phongInfo.specular));
    mShininess.append(QVector<float>(3, phongInfo.shininess));

    mTriangleNormals.append(t.normal());
    mTriangles.append({firstIdx, firstIdx + 1, firstIdx + 2});
    mTriangleTextureCoordinates.append({t.textureCoords()[0],
                                        t.textureCoords()[1],
                                        t.textureCoords()[2]});
}

void PreObject::addPreObject(const PreObject &o)
{
    unsigned firstIdx = mVertexPositions.size();

    for (const SimpleTexturedObject::Triangle &t : o.mTriangles) {
        mTriangles.append({
                              firstIdx + t.getFirst(),
                              firstIdx + t.getSecond(),
                              firstIdx + t.getThird()
                          });
    }

    mVertexPositions.append(o.mVertexPositions);
    mTriangleNormals.append(o.mTriangleNormals);
    mReflAmbient.append(o.mReflAmbient);
    mReflDiffuse.append(o.mReflDiffuse);
    mReflSpecular.append(o.mReflSpecular);
    mShininess.append(o.mShininess);
    mTriangleTextureCoordinates.append(o.mTriangleTextureCoordinates);
}


QSharedPointer<SimpleTexturedObject> PreObject::toObject() const
{
    QSharedPointer<SimpleTexturedObject> obj = QSharedPointer<SimpleTexturedObject>::create();

    obj->setTriangleInfo(mVertexPositions, mTriangleNormals, mTriangles);
    obj->setMaterialInfo(mReflAmbient, mReflDiffuse, mReflSpecular, mShininess);
    obj->setTextureInfo(mTriangleTextureCoordinates, mImage);

    obj->commit();

    return obj;
}
/* END PartialMeshData::PreObject */

Trig::Trig(ImageInfo texture, PhongInfo material, QVector3D v1, QVector2D t1, QVector3D v2, QVector2D t2, QVector3D v3, QVector2D t3)
    : mV1(v1), mV2(v2), mV3(v3)
    , mT1(t1), mT2(t2), mT3(t3)
    , mTexture(texture)
    , mMaterial(material)
{
    mNormal = QVector3D::crossProduct(mV3 - mV2, mV1 - mV2).normalized();
}
