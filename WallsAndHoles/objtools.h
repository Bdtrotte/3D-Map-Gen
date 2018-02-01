#ifndef OBJTOOLS_H
#define OBJTOOLS_H
#include "renderableobject.h"
#include <QFile>
#include <QTextStream>
#include <cassert>

typedef QSharedPointer<RenderableObject> RenderableObjectP;

inline RenderableObjectP loadOBJ(QString path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;

    QTextStream in(&file);
    QVector<QVector3D> vertices;
    QVector<QVector2D> uvs;
    QVector<QVector3D> normals;
    QVector<unsigned int> vertexIndices;
    QVector<unsigned int> uvIndices;
    QVector<unsigned int> normalIndices;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.size()<1) continue;
        QStringList list = line.split(" ", QString::SkipEmptyParts);

        if(list[0]=="v"){
            assert(list.size()==4);
            vertices.push_back(QVector3D{
                list[1].toFloat(),
                list[2].toFloat(),
                list[3].toFloat()
            });
        }

        if(list[0]=="vt"){
            assert(list.size()==3);
            uvs.push_back(QVector2D{
                list[1].toFloat(),
                list[2].toFloat(),
            });
        }
        if(list[0]=="vn"){
            assert(list.size()==4);
            normals.push_back(QVector3D{
                list[1].toFloat(),
                list[2].toFloat(),
                list[3].toFloat()
            });
        }

        if(list[0]=="f"){
            assert(list.size()==4);
            for(int i=1; i<list.size(); i++){
                QStringList indexVec = list[i].split('/');
                assert(indexVec.size()==3);
                vertexIndices.push_back(indexVec[0].toInt()-1);
                uvIndices.push_back(indexVec[1].toInt()-1);
                normalIndices.push_back(indexVec[2].toInt()-1);
            }
        }
    }


    /*
     * In RenderableObject, each triangle index corresponds to an index into ALL of the vertex
     * arrays simultaneously. There are no separate vertex/normal/UV indices.
     *
     * This part of the function may add new elements to the vertex/normal/uv arrays to
     * accomodate for this.
     * */
    QVector<QVector3D> pVertices;
    QVector<QVector3D> pNormals;
//    QVector<QVector2D> pUV;
    QVector<unsigned int> pTriangleIndices;

    QVector<QPair<int, int>> indexPairToIndex;

    // Loop through
    for (int i = 0; i < vertexIndices.size(); ++i) {
        int vidx = vertexIndices[i];
        int nidx = normalIndices[i];

        int idx = indexPairToIndex.indexOf(QPair<int, int>(vidx, nidx));

        // If this vertex/normal pair hasn't been used yet, create new entries.
        if (idx == -1) {
            pVertices.append(vertices[vidx]);
            pNormals.append(normals[nidx]);

            idx = indexPairToIndex.size();
            indexPairToIndex.append(QPair<int, int>(vidx, nidx));
        }

        // Note, at this point pVertices[idx] and pNormals[idx] are the position
        // and normal of the i_th vertex.
        pTriangleIndices.append(idx);
    }


    //RenderableObject only support vertex now. Add full support in future.
    RenderableObjectP object = RenderableObjectP::create(pVertices, pNormals, pTriangleIndices);
    return object;
}

inline bool saveOBJ(QString path, RenderableObjectP object){
    qDebug() << "Saving obj...";
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&file);
    QVector<QVector3D> vertices  = object->getVertexData();
    QVector<unsigned int> vertexIndices = object->getTriangleIndices();
    QVector<QVector3D> normals = object->getVertexNormals();
    QVector<unsigned int> normalIndices = object->getTriangleIndices();
    /*uv is not supported yet*/
    QVector<QVector3D> uvs = object->getVertexNormals();
    QVector<unsigned int> uvIndices = object->getTriangleIndices();
    //
    assert(vertexIndices.size()%3==0);
    for(auto const& vert: vertices){
        out << "v";
        for(int i=0; i<3; i++){
            out << " " << vert[i];
        }
        out << endl;
    }
    for(auto const& uv: uvs){
        out << "vt";
        for(int i=0; i<3; i++){
            out << " " << uv[i];
        }
        out << endl;
    }
    for(auto const& norm: normals){
        out << "vn";
        for(int i=0; i<3; i++){
            out << " " << norm[i];
        }
        out << endl;
    }
    for(int i=0; i<vertexIndices.size(); i++){
        if(i%3==0) out << "f";
        out << ' ' << vertexIndices[i];
        out << '/' << uvIndices[i];
        out << '/' << normalIndices[i];
        if(i%3==2) out << endl;
    }
    file.close();
    return true;
}

#endif // OBJTOOLS_H
