#ifndef OBJTOOLS_H
#define OBJTOOLS_H
#include <QFile>
#include <QTextStream>
#include <cassert>

#include "simpletexturedobject.h"

typedef QSharedPointer<SimpleTexturedObject> SharedSimpleTexturedObject;


/* TODO
 *
 * 1) This method should be moved into a special class, possibly something
 * like "SimpleTexturedObjectSaver".
 *
 * 2) This method does not load texture information.
 * */
/*
inline SimpleTexturedObjectP loadOBJSimpleTextured(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;

    // Data collected from the .obj file.
    QVector<QVector3D> vertices;    // v
    QVector<QVector2D> uvs;         // vt
    QVector<QVector3D> normals;     // vn

    // Data corresponding to lines starting with f.
    QVector<SimpleTexturedObject::Triangle> vertexIndices;
    QVector<SimpleTexturedObject::Triangle> uvIndices;
    QVector<SimpleTexturedObject::Triangle> normalIndices;


    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        // Ignore empty lines.
        if(line.size() <= 0)
            continue;

        // Split the line into its components.
        QStringList list = line.split(" ", QString::SkipEmptyParts);

        // Parse the line depending on its first component.

        if (list[0] == "v") {
            // Vertex.
            Q_ASSERT(list.size() == 4);

            vertices.push_back(QVector3D(
                list[1].toFloat(),
                list[2].toFloat(),
                list[3].toFloat()
            ));
        }

        else if (list[0] == "vt") {
            // Texture coordinates.
            Q_ASSERT(list.size() == 3);

            uvs.push_back(QVector2D(
                list[1].toFloat(),
                list[2].toFloat()
            ));
        }

        else if (list[0] == "vn") {
            // Normals.
            Q_ASSERT(list.size() == 4);

            normals.push_back(QVector3D(
                list[1].toFloat(),
                list[2].toFloat(),
                list[3].toFloat()
            ));
        }

        else if (list[0] == "f") {
            // Faces. Each face must be a triangle.
            Q_ASSERT(list.size() == 4);

            QStringList indexVec1 = list[1].split('/');
            QStringList indexVec2 = list[2].split('/');
            QStringList indexVec3 = list[3].split('/');

            // Vertex, tex. coord, and normal info must be supplied for each vertex.
            Q_ASSERT( indexVec1.size() == 3 );
            Q_ASSERT( indexVec2.size() == 3 );
            Q_ASSERT( indexVec3.size() == 3 );


            // Subtract 1 because .obj indices are 1-based.

            vertexIndices.push_back({
                                        indexVec1[0].toUInt() - 1,
                                        indexVec2[0].toUInt() - 1,
                                        indexVec3[0].toUInt() - 1
                                    });

            normalIndices.push_back({
                                        indexVec1[1].toUInt() - 1,
                                        indexVec2[1].toUInt() - 1,
                                        indexVec3[1].toUInt() - 1
                                    });
            uvIndices.push_back({
                                    indexVec1[2].toUInt() - 1,
                                    indexVec2[2].toUInt() - 1,
                                    indexVec3[2].toUInt() - 1
                                });
        }
    }


     * Reformat the .obj data to the format required by SimpleTexturedObject.
     * That is,
     *  1) Vertices remain the same.
     *  2) Normals are per-triangle.
     *  3) UVs are per-vertex-per-triangle (TriangleTexCoords structure).
     *  4) Triangle vertex indices are given in a Triangle structure.

    QVector<QVector3D> &pVertices = vertices;
    QVector<SimpleTexturedObject::Triangle> &pTriangles = vertexIndices;

    QVector<QVector3D> pFaceNormals;
    QVector<SimpleTexturedObject::TriangleTexCoords> pUV;



    // Loop through all faces.
    for (int triangleIdx = 0; triangleIdx < vertexIndices.size(); ++triangleIdx) {

        auto faceNormalIndices = normalIndices[triangleIdx];
        QVector3D faceVertexNormals[3] = {
            normals[faceNormalIndices.getFirst()],
            normals[faceNormalIndices.getSecond()],
            normals[faceNormalIndices.getThird()]
        };

        auto faceUVIndices = uvIndices[triangleIdx];

        // For the face normal, just average out the vertex normals.
        pFaceNormals.push_back((faceVertexNormals[0] + faceVertexNormals[1] + faceVertexNormals[2]).normalized());

        pUV.push_back({
                          uvs[faceUVIndices.getFirst()],
                          uvs[faceUVIndices.getSecond()],
                          uvs[faceUVIndices.getThird()]
                      });
    }


    SimpleTexturedObjectP object = SimpleTexturedObjectP::create();

    object->setTriangleInfo(pVertices, pFaceNormals, pTriangles);
    object->setMaterialInfo(1, 1, 1, 1); // soft material default
    object->setTextureInfo(pUV, nullptr);
    object->commit();

    return object;
}
*/

/* TODO
 *
 * Just like the load function above, this should be moved to a different class
 * and needs to be modified to handle texture information.
 * */
/*
inline bool saveOBJSimpleTextured(QString path, const SimpleTexturedRenderer &object)
{
    qDebug() << "Saving obj...";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    const auto &vertices = object.getVertices();
    const auto &faceNormals = object.getFaceNormals();
    const auto &triangles = object.getTriangles();
    const auto &faceTexCoords = object.getFaceTexCoords();



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

    return true;
}
*/
/*
namespace obj
{
    // Structure: Vertex
    //
    // Description: Model Vertex object that holds
    //	a Position, Normal, and Texture Coordinate
    struct Vertex
    {
        // Position Vector
        QVector3D position;

        // Normal Vector
        QVector3D normal;

        // Texture Coordinate Vector
        QVector2D textureCoordinate;
    };

    // Structure: Mesh
    //
    // Description: A Simple Mesh Object that holds
    //	a name, a vertex list, and an index list
    struct Mesh
    {
        // Default Constructor
        Mesh()
        {

        }
        // Variable Set Constructor
        Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
        {
            Vertices = _Vertices;
            Indices = _Indices;
        }
        // Mesh Name
        QString meshName;
        // Vertex List
        QVector<Vertex> vertices;
        // Index List
        QVector<unsigned int> indices;

        // Material
        QString materialName;
    };
}
*/
class Material
{
    Material(QString _name, int _Ns, int _illum)
    {
        name = _name;
        Ns = _Ns;
        illum = _illum;
    }

private:
    // Material Name
    QString name;
    // Ambient Color
    QVector3D Ka;
    // Diffuse Color
    QVector3D Kd;
    // Specular Color
    QVector3D Ks;
    // Specular Exponent
    float Ns;
    // Illumination
    int illum;
    // Name of Ambient Texture Map
    QString map_Ka;
    // Name of Diffuse Texture Map
    QString map_Kd;
};

class OBJModel
{
public:
    ~OBJModel(){
        mObjects.clear();
    }
    void addSimpleTextured(SharedSimpleTexturedObject object);
    void saveOBJ(QString path);

private:
    QVector<SharedSimpleTexturedObject> mObjects;
    QString mSaveDirectory;
};

typedef QSharedPointer<OBJModel> SharedOBJModel;

#endif // OBJTOOLS_H
