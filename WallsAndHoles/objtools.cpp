#include "objtools.h"
#include "triplet.h"

using Triangle = Triplet<unsigned int, unsigned int, unsigned int>;
using TriangleTexCoords = Triplet<QVector2D, QVector2D, QVector2D>;

void OBJModel::addSimpleTextured(const SimpleTexturedObject *object)
{
    /* interface of SimpleTexturedObject
    int getNumVertices() const;
    int getNumTriangles() const;
    const QVector<QVector3D> &getVertices() const;
    const QVector<QVector3D> &getFaceNormals() const;
    const QVector<Triangle> &getTriangles() const;

    const QVector<float> &getVertexAmbient() const;
    const QVector<float> &getVertexDiffuse() const;
    const QVector<float> &getVertexSpecular() const;
    const QVector<float> &getVertexShininess() const;

    const QVector<TriangleTexCoords> &getFaceTexCoords() const;
    const QImage &getImage() const;
    */
    mObjects.append(object);
}

void OBJModel::saveOBJ(QString path)
{
    //qDebug() << "Saving obj...";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QVector<QVector3D> vertices;
    QVector<QVector3D> faceNormals;
    QVector<Triangle> triangles;
    QVector<TriangleTexCoords> faceTexCoords ;

    int numVertices=1;
    //int numTriangles=0;
    for(auto object: mObjects){
        vertices += object->getVertices();
        faceNormals += object->getFaceNormals();
        for(auto triangle: object->getTriangles()){
            Triangle newTriangle(
                        numVertices+triangle.getFirst(),
                        numVertices+triangle.getSecond(),
                        numVertices+triangle.getThird());
            triangles.append(newTriangle);
        }
        faceTexCoords += object->getFaceTexCoords();
        numVertices += object->getNumVertices();
    }

    QTextStream out(&file);

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
    for (int faceIdx = 0; faceIdx < triangles.size(); ++faceIdx) {
        const auto &face = triangles[faceIdx];

        out << "f";

        unsigned int vertexIndices[3] = {face.getFirst(), face.getSecond(), face.getThird()};

        for (int idx = 0; idx < 3; ++idx) {
            out << " " << vertexIndices[idx];
            out << "/" << faceIdx*3 + idx;      // texture coordinate; see tex coord output above
            out << "/" << faceIdx;              // one normal per face
        }

        out << endl;
    }

    file.close();
}
