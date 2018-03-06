#include "objtools.h"
#include "triplet.h"

using Triangle = Triplet<unsigned int, unsigned int, unsigned int>;
using TriangleTexCoords = Triplet<QVector2D, QVector2D, QVector2D>;

QString vec3D2Str(QVector3D vec)
{
    return QString("%1 %2 %3").arg(
        QString::number(vec[0]),
        QString::number(vec[1]),
        QString::number(vec[2])
    );
}

QString Material::serialize()
{
    QString mtl="newmtl "+name;
    mtl = mtl + "\n";
    mtl = mtl + "Ka " + vec3D2Str(Ka) + "\n";
    mtl = mtl + "Kd " + vec3D2Str(Kd) + "\n";
    mtl = mtl + "Ks " + vec3D2Str(Ks) + "\n";
    mtl = mtl + "Ns " + QString::number(Ns) + "\n";
    mtl = mtl + "illum  " + QString::number(illum) + "\n";
    mtl = mtl + "map_Ka " + Map_Ka + "\n";
    mtl = mtl + "map_Kd " + Map_Kd + "\n";
    return mtl;
}

void OBJModel::addSimpleTextured(SharedSimpleTexturedObject obj)
{
    mObjects.append(obj);

    QString materialName = obj->getMaterialName();
    if(!mMaterials.contains(materialName)){
        QString name = materialName;

        QVector3D Ka(obj->getAmbient(),obj->getAmbient(), obj->getAmbient());
        QVector3D Kd(obj->getDiffuse(),obj->getDiffuse(),obj->getDiffuse());
        QVector3D Ks(obj->getSpecular(),obj->getSpecular(),obj->getSpecular());
        int Ns=obj->getShininess();
        int illum=2;

        QString imagePath = obj->getImageAndSource()->source();
        QString imageName = imagePath.mid(imagePath.lastIndexOf('/'));

        QString KaImage = imageName;
        QString KdImage = imageName;

        mMaterials[materialName]=SharedMaterial::create(name, Ka, Kd, Ks, Ns, illum, KaImage, KdImage);

        if(!mImages.contains(imageName)){
            mImages[imageName]=obj->getImageAndSource()->image();
        }
    }
}

void OBJModel::saveOBJ(QString path)
{
    //qDebug() << "Saving obj...";

    QFile objFile(path);
    if (!objFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QVector<QVector3D> vertices;
    QVector<QVector3D> faceNormals;
    QVector<QVector<Triangle>> meshs;
    QVector<TriangleTexCoords> faceTexCoords;

    int numVertices=1;
    //int numTriangles=0;
    for(auto object: mObjects){
        vertices += object->getVertices();
        faceNormals += object->getFaceNormals();
        QVector<Triangle> mesh;
        for(auto triangle: object->getTriangles()){
            Triangle newTriangle(
                        numVertices+triangle.getFirst(),
                        numVertices+triangle.getSecond(),
                        numVertices+triangle.getThird());
            mesh.append(newTriangle);
        }
        meshs.append(mesh);
        faceTexCoords += object->getFaceTexCoords();
        numVertices += object->getNumVertices();
    }

    QTextStream out(&objFile);

    out << "mtllib "+ name + ".mtl" << endl;

    // Write the list of vertices.
    for (auto const &vert : vertices) {
        out << "v";
        for (int i = 0; i < 3; i++)
            out << " " << vert[i];
        out << endl;
    }

    // Write the list of texture coordinates.
    // Note: For vertex j of triangle i, its texture coordinates are at (0-based) index
    //              3*i + j
    for (auto const &texCoords : faceTexCoords) {

        // texCoords is a Triplet of QVector2D
        QVector2D uvs[3] = {texCoords.getFirst(), texCoords.getSecond(), texCoords.getThird()};

        for (int i = 0; i < 3; ++i) {
            const auto &uv = uvs[i];

            out << "vt";
            for (int i = 0; i < 2; i++)
                out << " " << uv[i];
            out << endl;
        }
    }

    // Write the list of face normals.
    for (auto const &faceNorm : faceNormals) {
        out << "vn";
        for (int i = 0; i < 3; i++)
            out << " " << faceNorm[i];
        out << endl;
    }

    // Write the list of faces.
    QString oldMaterial = "";
    int faceIdStart = 1;
    for(int i=0; i<mObjects.length(); i++){
        QVector<Triangle> triangles = meshs[i];
        QString newMaterial = mObjects[i]->getMaterialName();
        if(newMaterial!=oldMaterial){
            out << "usemtl " << mObjects[i]->getMaterialName() << endl;
            oldMaterial = newMaterial;
        }
        for (int faceIdx = faceIdStart; faceIdx < faceIdStart+triangles.size(); ++faceIdx) {
            const auto &face = triangles[faceIdx-faceIdStart];

            out << "f";

            unsigned int vertexIndices[3] = {face.getFirst(), face.getSecond(), face.getThird()};

            for (int idx = 0; idx < 3; ++idx) {
                out << " " << vertexIndices[idx];
                out << "/" << (faceIdx-1)*3 + idx+1;      // texture coordinate; see tex coord output above
                out << "/" << faceIdx;              // one normal per face
            }

            out << endl;
        }
        faceIdStart+=triangles.size();
    }
    objFile.close();
}

void OBJModel::saveMTL(QString path)
{
    QFile mtlFile(path);
    if (!mtlFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&mtlFile);

    for(SharedMaterial material: mMaterials.values()){
        out << material->serialize() << endl;
    }
    mtlFile.close();
}

void OBJModel::saveImages(QString directory)
{
    for(QString imageName: mImages.keys()){
        QString path = directory + imageName;
        mImages[imageName]->save(path);
    }
}

void OBJModel::save(QString path)
{
    //parse path into directory and file name
    QStringList pathList = path.split('/');
    QString objName = pathList[pathList.length()-1];
    name = objName.split('.')[0];
    pathList.removeLast();
    mSaveDirectory = pathList.join('/');

    saveOBJ(path);
    saveMTL(mSaveDirectory+"/"+name+".mtl");
    saveImages(mSaveDirectory);
}
