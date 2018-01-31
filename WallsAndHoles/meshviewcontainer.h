#ifndef MESHVIEWCONTAINER_H
#define MESHVIEWCONTAINER_H

#include <QWidget>

#include "meshview.h"

namespace Ui {
class MeshViewContainer;
}

class MeshViewContainer : public QWidget
{
    Q_OBJECT

public:
    explicit MeshViewContainer(QWidget *parent = 0);
    ~MeshViewContainer();
    void saveMesh(QString path);
    void loadMesh(QString path);

    void setScene(QSharedPointer<Scene> scene);

private slots:
    void on_toolSelection_currentIndexChanged(int index);


protected:

    MeshView *mMeshView;

private:
    Ui::MeshViewContainer *ui;
};

#endif // MESHVIEWCONTAINER_H
